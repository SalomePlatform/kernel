#! /bin/sh

echo "-----------------------> Daemon notification..."

notifd -c ${SALOME_SRC_DIR}/resources/channel.cfg -DFactoryIORFileName=/tmp/${LOGNAME}_rdifact.ior -DChannelIORFileName=/tmp/${LOGNAME}_rdichan.ior&
sleep 2
