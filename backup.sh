#!/bin/sh

tar cyf solutions.zip -C ./src --exclude .DS_Store solutions
gpg --output solutions.gpg --encrypt --armor --recipient dparillo@sdccd.edu 
rm solutions.zip


