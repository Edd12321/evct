#!/usr/bin/env sh

if [ $(basename $(pwd)) = "scripts" ]
then
      cd ..
fi

ua="$(uname -s)"
case "${ua}" in
    CYGWIN*) cp scripts/start_ui.bat bin/Release/start_ui.bat;;
          *) ;;
esac

cp evct.conf bin/Release/evct.conf
cp "test.txt" "bin/Release/test.txt"

cp gui.py bin/Release/gui.py
chmod 777 bin/Release/gui.py
