#! /bin/sh

#automates the creation of the needed media
#usage: ./make_dist_media.sh <media svn directory> <local directory> <image pixel max with/height>
#example: ./make_dist_media.sh media-svn-dir media-dev 256
#note that the script must be run in the ember source root dir

if [ $# -ne 3 ]
then
  echo "This script will automatically scan the media source repo and create a repository suitable for distributions."
  echo "This process mainly involves copying only those assets which are used by Ember, and resizing textures."
  echo "Usage: `basename $0` <media svn directory> <local directory> <image pixel max with/height>"
  exit -1
fi

hash dos2unix 2>&- || { echo >&2 "I require dos2unix but it's not installed.  Aborting."; exit 1; }
hash convert 2>&- || { echo >&2 "I require convert (ImageMagick) but it's not installed.  Aborting."; exit 1; }

current=${PWD}
original_media=`readlink -m $1`
media_dir=`readlink -m $2`
textureSize=$3
shared_dir=${media_dir}/media/shared
shared_common_dir=${shared_dir}/common

common_textures_list=`mktemp --tmpdir common_textures_list.XXX`
common_textures_cleaned_list=`mktemp --tmpdir common_textures_cleaned_list.XXX`
common_meshes_list=`mktemp --tmpdir common_meshes_list.XXX`
common_skeletons_list=`mktemp --tmpdir common_skeletons_list.XXX`
shared_packs_list=`mktemp --tmpdir shared_packs_list.XXX`
common_sounds_list=`mktemp --tmpdir common_sounds_list.XXX`
media_textures_list=`mktemp --tmpdir media_textures_list.XXX`



srcdir=${PWD}

ogre_dir=${original_media}/resources/ogre
script_dir=${ogre_dir}/scripts
material_dir=${script_dir}/materials
overlay_dir=${script_dir}/overlays
program_dir=${script_dir}/programs

#start with shared media

mkdir -p ${shared_common_dir}
cd ${shared_dir}

#try to get the textures needed
#don't include materials in the ogre or ogre core
for filename in `find ${original_media} -name *.material`
do
  grep -IE --no-filename "^[^/].*texture "  ${filename} | sed -e 's/texture //g' >> ${common_textures_list}
  grep -IE --no-filename "^[^/].*set_texture_alias DiffuseMap "  ${filename} | sed -e 's/set_texture_alias DiffuseMap //g' >> ${common_textures_list}
  grep -IE --no-filename "^[^/].*set_texture_alias NormalHeightMap "  ${filename} | sed -e 's/set_texture_alias NormalHeightMap //g' >> ${common_textures_list}
  grep -IE --no-filename "^[^/].*set_texture_alias SpecularMap "  ${filename}| sed -e 's/set_texture_alias SpecularMap //g' >> ${common_textures_list}
done

#Parse all particle scripts and look for image references
for filename in `find ${original_media} -name *.particle`
do
  grep -IE --no-filename "image\s*.*" ${filename} | sed -e 's/image\s*//g' >> ${common_textures_list}
done

grep -orIE --no-filename "icon=\"[^\"]*\"" ${srcdir}/src/components/ogre/data/*.modeldef | sed -e 's/icon=\"//g' | sed -e 's/\"//g' >> ${common_textures_list}
grep -orIE --no-filename "diffusetexture=\"[^\"]*\"" ${srcdir}/src/components/ogre/data/*.terrain | sed -e 's/diffusetexture=\"//g' | sed -e 's/\"//g' >> ${common_textures_list}
grep -orIE --no-filename "normalmaptexture=\"[^\"]*\"" ${srcdir}/src/components/ogre/data/*.terrain | sed -e 's/normalmaptexture=\"//g' | sed -e 's/\"//g' >> ${common_textures_list}

#remove all duplicates
sort --unique ${common_textures_list} | sed '/^$/d' | dos2unix > ${common_textures_cleaned_list}

#first copy all textures
# cd ${original_media}/common ; tar cf - `cat ${shared_dir}/common_textures_cleaned.list ` | ( cd ${shared_dir}/common; tar xvf -)

#then convert them to a little lower smaller
echo "Converting images"
for filename in `cat ${common_textures_cleaned_list} `
do
	origfile="${original_media}/${filename// /}"
	newfile="${shared_common_dir}/${filename// /}"
	if [ ! -e $origfile ]
	then
		echo "File ${origfile} does not exist!"
	else
		#only convert if the original image is newer
		if [ $origfile -nt $newfile ]
		then
			mkdir -p `dirname ${newfile}`
			#'512x512>' will only convert if the image is larger in any dimension
			# -quality 95 is suitable for photos
			# -depth 8 guarantees that the pixeldepth is 8: we don't have any floating point textures yet
			#echo "convert $origfile -quality 95 -depth 8 -resize  $newfile"
			
			#For some reason (bug in Ogre? in ImageMagick? in FreeImage?) Ogre doesn't properly recognize
			#greyscale textures unless they have 256 colours. The "convert" call will optimize the images
			#and reduce the palette. To prevent it from doing this to greyscale images we must first find
			#those that are such, and then supply the "-colors 256" directive to "convert" in order for it
			#to preserve the palette.
			isgrayscale=`identify $origfile 2>/dev/null | grep -c '256c'`
			
			if [ "$isgrayscale" == '1' ]
			then
				colourdirective=" -colors 256 "
			fi
			
			convert $origfile -quality 95 -depth 8 $colourdirective -resize "${textureSize}x${textureSize}>" $newfile
			echo "Converted $filename to max ${textureSize}x${textureSize}"
		fi
	fi
	
done

#copy all meshes in use
echo "Copying meshes"
cd ${shared_dir}
grep -orIE --no-filename "mesh=\"[^\"]*\"" ${srcdir}/src/components/ogre/data/*.modeldef | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' > ${common_meshes_list}
# grep -orIE --no-filename "mesh=\"junk[^\"]*\"" ${original_media}/data/*.modeldef | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' >> ${common_meshes_list}
grep -orIE --no-filename "mesh=\"[^\"]*\"" ${srcdir}/src/components/ogre/data/*.modeldef.xml | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' >> ${common_meshes_list}
# grep -orIE --no-filename "mesh=\"junk[^\"]*\"" ${original_media}/data/*.modeldef.xml | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' >> ${common_meshes_list}
cd ${original_media} ; tar cf - `cat ${common_meshes_list} ` | ( cd ${shared_common_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
# exit
# mkdir -p ${shared_dir}/models
# cd ${original_media}/models ; tar cf - `find -L . -iname \*.mesh` | ( cd ${shared_dir}/models; tar --keep-newer-files -xvf -) 2>  /dev/null

echo "Copying skeletons"
#just copy all skeletons, since the method involving meshmagick can't resolve the correct file name when using relative skeleton names
cd ${original_media}/3d_objects ; tar cf - `find -L . -iname \*.skeleton` | ( cd ${shared_common_dir}/3d_objects; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${original_media}/3d_skeletons ; tar cf - `find -L . -iname \*.skeleton` | ( cd ${shared_common_dir}/3d_skeletons; tar --keep-newer-files -xvf -) 2>  /dev/null

# #use meshmagick to figure out the needed skeletons
# for filename in `find ${shared_dir}/common -name "*.mesh"`
# do
# 	meshmagick info ${filename} | grep -oE "skeletonfile.*.skeleton" | sed -e 's/skeletonfile //' >> ${common_skeleton_list}
# 	meshmagick info ${filename} | grep -oE "skeleton file.*.skeleton" | sed -e 's/skeleton file //' >> ${common_skeleton_list}
# done
# cd ${original_media}/common ; tar cf - `cat ${common_skeleton_list} ` | ( cd ${shared_dir}/common; tar --keep-newer-files -xvf -) 2>  /dev/null



#copy all sounds in use
echo "Copying sounds"
cd ${shared_dir}
grep -orIE --no-filename "filename=\"[^\"]*\"" ${srcdir}/src/components/ogre/sounddefinitions/*.sounddef | sed -e 's/filename=\"//g' | sed -e 's/\"//g' > ${common_sounds_list}
cd ${original_media} ; tar cf - `cat ${common_sounds_list} ` | ( cd ${shared_common_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null


cd ${shared_dir}
cp -a ${original_media}/LICENSING.txt .
cp -a ${original_media}/COPYING.txt .
cd ${shared_common_dir}
mkdir -p ./resources/ogre/ogre_scripts
cp -a ${original_media}/resources/ogre/ogre_scripts/COPYING ./resources/ogre/ogre_scripts/

# echo "Copying sounds"
# mkdir -p ${shared_dir}/sounds
# cd ${original_media}/sounds; tar cf - `find . -iname \*.wav` | ( cd ${shared_dir}/sounds; tar xvf -)


echo "Copying gui files"
cd ${shared_dir}
grep -orIE --no-filename "Imagefile=\"[^\"]*\"" ${srcdir}/src/components/cegui/datafiles/imagesets/*.imageset | sed -e 's/Imagefile=\"//g' | sed -e 's/\"//g' > ${media_textures_list}
cd ${original_media} ; tar cf - `cat ${media_textures_list} ` | ( cd ${shared_dir}/common; tar --keep-newer-files -xvf -) 2>  /dev/null


echo "Copying shared media packs"
cd ${shared_dir}
grep -rIE --no-filename "^Zip\[shared\]=.*" ${current}/src/components/ogre/resources.cfg | sed -e 's/Zip\[shared\]=//g' > ${shared_packs_list}
cd ${original_media} ; tar cf - `cat ${shared_packs_list} ` | ( cd ${shared_dir}/; tar --keep-newer-files -xvf -) 2>  /dev/null

echo "Copy fonts"
mkdir -p ${shared_dir}/common/themes/ember/gui/fonts
cp -a ${original_media}/themes/ember/gui/fonts/* ${shared_dir}/common/themes/ember/gui/fonts

echo "Copying materials"

cd ${original_media}
for filename in `find ${original_media} -iname \*.cg -o -iname \*.glsl -o -iname \*.hlsl -o -iname \*.program -o -iname \*.asm -o -iname \*.ps -o -iname \*.material -o -iname \*.overlay -o -iname \*.particle -o -iname \*.compositor`
do
	filename=${filename#$original_media/}
	origfile="${original_media}/${filename#.\/}"
	newfile="${shared_common_dir}/${filename#.\/}"
	#only copy if the original file is newer
	if [ $origfile -nt $newfile ]
	then
		if [  ! -e `dirname $newfile` ]; then
			mkdir -p `dirname $newfile`
		fi
		cp -uf $origfile $newfile
		echo "Copied $filename to $newfile"
	fi
done


shared_ogre_dir=${shared_common_dir}/resources/ogre
#Note: we'll keep this for now, until we have a nicer way of copying all the textures that's needed
mkdir -p ${shared_ogre_dir}/textures
cd ${ogre_dir}/textures ; tar cf - `find . -iname \*.png` | ( cd ${shared_ogre_dir}/textures; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir}/textures ; tar cf - `find . -iname \*.jpg` | ( cd ${shared_ogre_dir}/textures; tar --keep-newer-files -xvf -) 2>  /dev/null

cd ${original_media}
#Remove any directories which are marked "norecurse".
for filename in `find . -name norecurse`
do
  directory=`dirname ${filename#./}`
  shared_media_remove_dir=$shared_common_dir/$directory
  echo "Removing ${shared_media_remove_dir}"
  rm -rf ${shared_media_remove_dir}
done


echo "Copying core files"
mkdir -p ${shared_dir}/core
for filename in `cat ${srcdir}/media/core/EmberCore.files `
do
	echo "Copying core file ${original_media}/${filename}"
	cp -uf ${original_media}/${filename} ${shared_dir}/core
done

echo "Copying Caelum files"
#don't include any hidden directories since they are subversion directories
mkdir -p ${shared_dir}/common/resources/ogre/caelum
cd ${original_media}/resources/ogre/caelum
for filename in `find . ! -regex '.*/\..*' `
do
	echo "Copying Caelum file  /resources/ogre/caelum/$filename"
	mkdir -p `dirname ${shared_common_dir}/resources/ogre/caelum/${filename}`
	cp -uf ${filename} ${shared_common_dir}/resources/ogre/caelum/${filename}
done


echo "Cleanup"
unlink ${common_textures_list}
unlink ${common_textures_cleaned_list}
unlink ${common_meshes_list}
unlink ${common_skeletons_list}
unlink ${shared_packs_list}
unlink ${common_sounds_list}
unlink ${media_textures_list}

echo "Done!"

