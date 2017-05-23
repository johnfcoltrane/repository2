#!/bin/bash
# ローカルにリポジトリ作成
mkdir MyGitRepo1
cd MyGitRepo1
git init --bare
cd ..

# ワークツリー作成
mkdir gitwork1
cd gitwork1/
git init

# ファイルを追加
touch a.txt
git add a.txt
git commit -m "add"

# リモート設定
git remote add origin ../MyGitRepo1

# push
git push origin master

