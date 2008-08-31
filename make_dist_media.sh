#! /bin/sh

#automates the creation of the needed media
#usage: ./make_dist_media.sh <local directory> <image pixel max with/height>
#example: ./make_dist_media.sh media-dev 256
#note that the script must be run in the ember source root dir

current=${PWD}
media_dir=${current}/$1
textureSize=$2
original_media=${PWD}/media
user_dir=${media_dir}/media/user
shared_dir=${media_dir}/media/shared

ogre_dir=${original_media}/common/resources/ogre
script_dir=${ogre_dir}/scripts
material_dir=${script_dir}/materials

#start with shared media

mkdir -p ${shared_dir}/common
cd ${shared_dir}

#try to get the textures needed
#don't include lines starting with "/"
grep -rIE --no-filename "^[^/].*texture "  ${material_dir}/*.material | sed -e 's/texture //g' > common_textures.list
grep -rIE --no-filename "^[^/].*set_texture_alias DiffuseMap "  ${material_dir}/*.material | sed -e 's/set_texture_alias DiffuseMap //g' >> common_textures.list
# grep -rIE --no-filename "^[^/].*set_texture_alias DiffuseMap art\/3d"  ${original_media}/materials/scripts/*.material | sed -e 's/set_texture_alias DiffuseMap art\/3d_/art\/3d_/g' >> common_textures.list
# grep -rIE --no-filename "^[^/].*texture themes"  ${original_media}/materials/scripts/*.material | sed -e 's/texture themes/themes/g' >> common_textures.list
# grep -rIE --no-filename "^[^/].*set_texture_alias NormalMap 3d"  ${original_media}/materials/scripts/*.material | sed -e 's/set_texture_alias NormalMap 3d_/3d_/g' >> common_textures.list
grep -rIE --no-filename "^[^/].*set_texture_alias NormalHeightMap "  ${material_dir}/*.material | sed -e 's/set_texture_alias NormalHeightMap //g' >> common_textures.list
grep -rIE --no-filename "^[^/].*set_texture_alias SpecularMap "  ${material_dir}/*.material | sed -e 's/set_texture_alias SpecularMap //g' >> common_textures.list
grep -orIE --no-filename "icon=\"[^\"]*\"" ${original_media}/modeldefinitions/*.modeldef | sed -e 's/icon=\"//g' | sed -e 's/\"//g' >> common_textures.list
grep -orIE --no-filename "diffusetexture=\"[^\"]*\"" ${original_media}/modeldefinitions/*.terrain | sed -e 's/diffusetexture=\"//g' | sed -e 's/\"//g' >> common_textures.list
grep -orIE --no-filename "normalmaptexture=\"[^\"]*\"" ${original_media}/modeldefinitions/*.terrain | sed -e 's/normalmaptexture=\"//g' | sed -e 's/\"//g' >> common_textures.list

#remove all duplicates
sort --unique common_textures.list > common_textures_cleaned.list

#first copy all textures
# cd ${original_media}/common ; tar cf - `cat ${shared_dir}/common_textures_cleaned.list ` | ( cd ${shared_dir}/common; tar xvf -)

#then convert them to a little lower smaller
echo "Converting images"
for filename in `cat ${shared_dir}/common_textures_cleaned.list `
do
	origfile="${original_media}/common/${filename}"
	newfile="${shared_dir}/common/${filename}"
	if [ ! -e $origfile ]
	then
		echo "File $origfile does not exist!"
	else
		#only convert if the original image is newer
		if [ $origfile -nt $newfile ]
		then
			mkdir -p `dirname ${newfile}`
			#'512x512>' will only convert if the image is larger in any dimension
			# -quality 95 is suitable for photos
			# -depth 8 guarantees that the pixeldepth is 8: we don't have any floating point textures yet
			#echo "convert $origfile -quality 95 -depth 8 -resize  $newfile"
	
			convert $origfile -quality 95 -depth 8 -resize "${textureSize}x${textureSize}>" $newfile
			echo "Converted $filename to max ${textureSize}x${textureSize}"
		fi
	fi
	
done


#copy all meshes in use
echo "Copying meshes"
cd ${shared_dir}
grep -orIE --no-filename "mesh=\"[^\"]*\"" ${original_media}/modeldefinitions/*.modeldef | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' > ${shared_dir}/common_meshes.list
# grep -orIE --no-filename "mesh=\"junk[^\"]*\"" ${original_media}/modeldefinitions/*.modeldef | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' >> ${shared_dir}/common_meshes.list
grep -orIE --no-filename "mesh=\"[^\"]*\"" ${original_media}/modeldefinitions/*.modeldef.xml | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' >> ${shared_dir}/common_meshes.list
# grep -orIE --no-filename "mesh=\"junk[^\"]*\"" ${original_media}/modeldefinitions/*.modeldef.xml | sed -e 's/mesh=\"//g' | sed -e 's/\"//g' >> ${shared_dir}/common_meshes.list
cd ${original_media}/common ; tar cf - `cat ${shared_dir}/common_meshes.list ` | ( cd ${shared_dir}/common; tar --keep-newer-files -xvf -) 2>  /dev/null
# exit
mkdir -p ${shared_dir}/models
cd ${original_media}/models ; tar cf - `find -L . -iname \*.mesh` | ( cd ${shared_dir}/models; tar --keep-newer-files -xvf -) 2>  /dev/null


echo "Copying skeletons"
#use meshmagick to figure out the needed skeletons
for filename in `find ${shared_dir}/common -name "*.mesh"`
do
	meshmagick info ${filename} | grep -oE "skeletonfile.*.skeleton" | sed -e 's/skeletonfile //' >> ${shared_dir}/common_skeletons.list
	meshmagick info ${filename} | grep -oE "skeleton file.*.skeleton" | sed -e 's/skeleton file //' >> ${shared_dir}/common_skeletons.list
done
cd ${original_media}/common ; tar cf - `cat ${shared_dir}/common_skeletons.list ` | ( cd ${shared_dir}/common; tar --keep-newer-files -xvf -) 2>  /dev/null



#copy all sounds in use
echo "Copying sounds"
cd ${shared_dir}
grep -orIE --no-filename "filename=\"[^\"]*\"" ${original_media}/sounddefinitions/*.sounddef | sed -e 's/filename=\"//g' | sed -e 's/\"//g' > ${shared_dir}/common_sounds.list
cd ${original_media}/common ; tar cf - `cat ${shared_dir}/common_sounds.list ` | ( cd ${shared_dir}/common; tar --keep-newer-files -xvf -) 2>  /dev/null


cd ${shared_dir}
cp -a ${original_media}/common/LICENSING.txt .
cp -a ${original_media}/common/COPYING.txt .

# echo "Copying sounds"
# mkdir -p ${shared_dir}/sounds
# cd ${original_media}/sounds; tar cf - `find . -iname \*.wav` | ( cd ${shared_dir}/sounds; tar xvf -)



echo "Copying shared media packs"
cd ${shared_dir}
grep -rIE --no-filename "^Zip\[shared\]=.*" ${current}/src/components/ogre/resources.cfg | sed -e 's/Zip\[shared\]=//g' > shared_packs.list
cd ${original_media} ; tar cf - `cat ${shared_dir}/shared_packs.list ` | ( cd ${shared_dir}/; tar --keep-newer-files -xvf -) 2>  /dev/null



#then get the user media

echo "User media"
mkdir -p ${user_dir}
cd ${user_dir}

#get the gui files
mkdir -p ${shared_dir}/gui/cegui
mkdir -p ${shared_dir}/gui/icons
cp -a ${original_media}/common/themes/ember/gui/cegui/EmberLook.png ${shared_dir}/gui/cegui
cp -a ${original_media}/common/themes/ember/gui/icons/ember.png ${shared_dir}/gui/icons
cp -a ${original_media}/common/themes/ember/gui/icons/iconset_mason.png ${shared_dir}/gui/icons
cp -a ${original_media}/common/themes/ember/gui/icons/iconset_standard.png ${shared_dir}/gui/icons
cp -a ${original_media}/common/themes/ember/gui/status ${shared_dir}/gui/status


#get the theme media dir
#mkdir -p ${current}/${media_dir}/media/common/themes/ember
#cd ${current}/media/common/themes/ember ; tar cf - `find . -iname \*.png` | ( cd ${current}/${media_dir}/media/common/themes/ember; tar xvf -)
#cd ${current}/media/common/themes/ember ; tar cf - `find . -iname \*.tga` | ( cd ${current}/${media_dir}/media/common/themes/ember; tar xvf -)
#cd ${current}/media/common/themes/ember ; tar cf - `find . -iname \*.jpeg` | ( cd ${current}/${media_dir}/media/common/themes/ember; tar xvf -)
#cd ${current}/media/common/themes/ember ; tar cf - `find . -iname \*.jpg` | ( cd ${current}/${media_dir}/media/common/themes/ember; tar xvf -)

echo "Copying definitions"
mkdir -p ${user_dir}/modeldefinitions
cd ${original_media}/modeldefinitions ; tar cf - `find . -iname \*.modeldef` | ( cd ${user_dir}/modeldefinitions; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${original_media}/modeldefinitions ; tar cf - `find . -iname \*.modeldef.xml` | ( cd ${user_dir}/modeldefinitions; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${original_media}/modeldefinitions ; tar cf - `find . -iname \*.modelmap` | ( cd ${user_dir}/modeldefinitions; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${original_media}/modeldefinitions ; tar cf - `find . -iname \*.modelmap.xml` | ( cd ${user_dir}/modeldefinitions; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${original_media}/modeldefinitions ; tar cf - `find . -iname \*.terrain` | ( cd ${user_dir}/modeldefinitions; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${original_media}/sounddefinitions ; tar cf - `find . -iname \*.sounddef` | ( cd ${user_dir}/sounddefinitions; tar --keep-newer-files -xvf -) 2>  /dev/null


echo "Copying materials"
user_ogre_dir=${user_dir}/common/resources/ogre
mkdir -p ${user_ogre_dir}
cd ${ogre_dir} ; tar cf - `find . -iname \*.png` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.jpg` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.cg` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.program` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.asm` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.ps` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.material` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null
cd ${ogre_dir} ; tar cf - `find . -iname \*.overlay` | ( cd ${user_ogre_dir}; tar --keep-newer-files -xvf -) 2>  /dev/null

#cd ${original_media}/models ; tar cf - `find . -iname \*.mesh` | ( cd ${user_dir}/models; tar xvf -)
#cd ${original_media}/models ; tar cf - `find . -iname \*.skeleton` | ( cd ${user_dir}/models; tar xvf -)
mkdir -p ${user_dir}/particle
cd ${original_media}/particle ; tar cf - `find . -iname \*.particle` | ( cd ${user_dir}/particle; tar --keep-newer-files -xvf -) 2>  /dev/null


echo "Copying core files"
mkdir -p ${shared_dir}/core
for filename in `cat ${original_media}/core/EmberCore.files `
do
	cp -uf ${original_media}/common/${filename} ${shared_dir}/core
done

echo "Copying Caelum files"
#don't include any hidden directories since they are subversion directories
mkdir -p ${shared_dir}/common/resources/ogre/caelum
cd ${original_media}/common/resources/ogre/caelum
for filename in `find . ! -regex '.*/\..*' `
do
	echo $filename
	mkdir -p `dirname ${shared_dir}/common/resources/ogre/caelum/${filename}`
	cp -uf ${filename} ${shared_dir}/common/resources/ogre/caelum/${filename}
done


echo "Copying base.zip file"
mkdir -p ${shared_dir}/common/resources/ogre/scripts
cp ${original_media}/common/resources/ogre/scripts/base.zip ${shared_dir}/common/resources/ogre/scripts/



echo "Copying user media packs"
cd ${user_dir}
grep -rIE --no-filename "^Zip\[user\]=.*" ${current}/src/components/ogre/resources.cfg | sed -e 's/Zip\[user\]=//g' > user_packs.list
cd ${original_media} ; tar cf - `cat ${user_dir}/user_packs.list ` | ( cd ${user_dir}/; tar --keep-newer-files -xvf -) 2>  /dev/null



cd ${user_dir}
cp -a ${original_media}/README .
cp -a ${original_media}/common/LICENSING.txt .
cp -a ${original_media}/common/COPYING.txt .

echo "Cleanup"
rm -f ${shared_dir}/common_textures.list
rm -f ${shared_dir}/common_textures_cleaned.list
rm -f ${shared_dir}/common_meshes.list
rm -f ${shared_dir}/common_skeletons.list
rm -f ${shared_dir}/shared_packs.list
rm -f ${shared_dir}/common_sounds.list


rm -f ${user_dir}/user_packs.list

echo "Done!"

