#!/bin/sh

DIRNAME=`dirname $0`
REALPATH=`which realpath`
if [ ! -z "${REALPATH}" ]; then
  DIRNAME=`realpath ${DIRNAME}`
fi

${HPHP_HOME}/hphp/hhvm/hhvm -a \
  -vDynamicExtensions.0=${DIRNAME}/lz4.so --config=/etc/hhvm/server.ini
