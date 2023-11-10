#!/bin/bash

# Get the version or hash control number using git describe
version_info=$(git describe --always --tags --dirty)

# Get a list of files that are about to be committed
files_to_commit=$(git diff --name-only)

echo "cmm=$files_to_commit $version_info > a.out"
pwwwd=$(pwd)

echo "pwd=$pwwwd > a.out"

# Loop through the list of files
for file in $files_to_commit; do
  # Check if the file contains the $Id$ marker
  if grep -q '\$Id \$' "$file"; then
    # Replace the $Id$ marker with the version or hash control number
    sed -i "s/\$Id \$/\$Id: $version_info \$/g" "$file"
  fi
done

# Exit with a successful status code to allow the commit to proceed
exit 0
