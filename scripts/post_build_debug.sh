#!/usr/bin/env sh

if [ $(basename $(pwd)) = "scripts" ]
then
    cd ..
fi

ua="$(uname -s)"
case "${ua}" in
    CYGWIN*) cp scripts/start_ui.bat bin/Debug/start_ui.bat;;
          *) ;;
esac

cp evct.conf bin/Debug/evct.conf
cp "test.txt" "bin/Debug/test.txt"

cp gui.py bin/Debug/gui.py
chmod 777 bin/Debug/gui.py
