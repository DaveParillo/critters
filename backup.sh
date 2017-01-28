#!/bin/sh

tar cyf solutions.zip -C ./src --exclude .DS_Store solutions
gpg --output solutions.gpg --encrypt --armor --recipient dparillo@sdccd.edu --recipient tashacv@gmail.com solutions.zip
rm solutions.zip


