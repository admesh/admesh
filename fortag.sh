#!/bin/bash
lasttag=''
total=''
for tag in `git for-each-ref --sort=committerdate --format='%(refname)' refs/tags`; do
  if [[ x$lasttag != x ]]; then
    e="${tag#refs/tags/v}: "
    e="$e`git  --no-pager log -1 --format=%ai $tag`\n\n"
    e="$e`git --no-pager log --pretty=\"format: * %s\" $lasttag..$tag`\n\n"
    total="$e$total"
  fi
  lasttag=$tag
done
echo -en "$total"
