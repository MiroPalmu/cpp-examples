#!/usr/bin/bash

declare -A cases

cases["by-value"]="vec2_array<${FLOAT_TYPE}, ${ARR_LEN}>"
cases["by-referece"]="${cases["by-value"]}&"

cases["by-const-value"]="const ${cases["by-value"]}"
cases["by-const-referece"]="const ${cases["by-value"]}&"

echo "CXX:       ${CXX}"
echo "CXX_FLAGS: ${CXX_FLAGS} -std=c++26 -Wall -Wextra"
echo

# Loop through the associative array
for key in "${!cases[@]}"; do
    "${CXX}" ${CXX_FLAGS} -std=c++26 main.cpp "-DARGUMENT_TYPE=${cases[$key]}" -o "${key}" \
        && echo "${key}:" && time "./${key}" 2500000
    rm -f "${key}"
done
