#!/usr/bin/env bash
commits=(
    32b2dc363740cbf9bbfcb953741293444a69e085
    3f981c7e4b67d1523c1039f7fed03073e10e0426
    fc221eeca5b0b7b47ea614a356786ce7d9f73105
    bd359bebeeb215f47060e6944e42f3c3f6e126fb
    09ef8dc233f8dc139a9b89b6352913a5fe1c3782
    a5a2235bab6bb73b8a24f10ee771bf47431b495a
    6792e00fbf913df39e488d5e00bc4c61740ae182
    c514503db78244967c4834d263e605d339d74999
    d8696e74ba87d32833cc41a4ae94b1d341932572
    219dfcd042d47d49091970b4f3cb5b4493adb994
)

descriptions=(
    "Original"
    "Use -march=native compile option"
    "Lift heap allocations out of hot loop"
    "Struct-of-arrays (SOA) transformation"
    "Try using float instead of double for vrsqrtps (GCC)"
    "Use OpenMP to parallelize middle loop"
    "Fix vectorization by switching to Clang"
    "Use std::swap instead of memcpy"
    "Try using float again, now using Clang"
    "Use -ffast-math, enabling rsqrt generation"
)

for index in "${!commits[@]}"
do
    git checkout "${commits[$index]}"
    echo "${descriptions[$index]}"
    make
    ./main.exe 1000 5000
    ./main.exe 5 10000000
    ./main.exe 1000 100000
done
