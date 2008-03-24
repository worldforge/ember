#! /bin/sh

#updates amber with the needed media. This will only work for me (Erik Hjortsberg) since only I know the password (as found in rsyncpasswd)

sh optimized/make_dist_media.sh media-dev 256
rsync -uavz --password-file rsyncpasswd  media-dev/media/* amber::media-dev-update