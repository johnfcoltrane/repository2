#!/bin/bash
# ���[�J���Ƀ��|�W�g���쐬
mkdir MyGitRepo1
cd MyGitRepo1
git init --bare
cd ..

# ���[�N�c���[�쐬
mkdir gitwork1
cd gitwork1/
git init

# �t�@�C����ǉ�
touch a.txt
git add a.txt
git commit -m "add"

# �����[�g�ݒ�
git remote add origin ../MyGitRepo1

# push
git push origin master

