#!/bin/sh

tar cyf solutions.zip -C ./src --exclude .DS_Store solutions
gpg --output solutions.gpg --encrypt --armor --recipient dparillo@sdccd.edu solutions.zip
rm solutions.zip


