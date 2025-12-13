#!usr/bin/env bash

<<comment
Each modules have their own test dir.
To make pio embedded test work, we will
symlink each test dir into pio's test dir.
comment

set -o pipefail

for module in src/modules/*; do 
    [ -d "$module/test" ] || continue
    
    module_name=$(basename "$module")
    
    link_name="test/test_${module_name}"
    
    ln -snf ../"$module"/test "$link_name"
done

pio test -e esp32_unit_test -vv
