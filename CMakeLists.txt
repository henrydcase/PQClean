cmake_minimum_required(VERSION 3.13)
project(cryptocore NONE)

enable_language(C)
enable_language(CXX)
enable_language(ASM)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "~/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "3rd/cmake-modules")
set(CMAKE_CXX_STANDARD 11)

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  set(ARCH "ARCH_x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
  set(ARCH "ARCH_x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
  set(ARCH "ARCH_x86_64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86")
  set(ARCH "ARCH_x86")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386")
  set(ARCH "ARCH_x86")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
  set(ARCH "ARCH_x86")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
  set(ARCH "ARCH_aarch64")
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
  set(ARCH "ARCH_aarch64")
else()
  message(FATAL_ERROR "Unknown processor:" ${CMAKE_SYSTEM_PROCESSOR})
endif()

# Arch settings

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CLANG 1)
endif()

if (MACOSX)
   set(CMAKE_C_COMPILER /usr/bin/cc CACHE PATH "" FORCE)
   set(CMAKE_CXX_COMPILER /usr/bin/c++ CACHE PATH "" FORCE)
endif()

# Global configuration

set(C_CXX_FLAGS
  "-Wno-ignored-qualifiers \
  -Wall \
  -Werror \
  -Wextra \
  -Wpedantic \
  -Wshadow \
  -Wno-variadic-macros \
  -Wundef \
  -Wunused-result")

if(CLANG)
  set(C_CXX_FLAGS
    "-Wconditional-uninitialized \
    -Wmissing-variable-declarations")
endif()

# Control Debug/Release mode
if(CMAKE_BUILD_TYPE_LOWER STREQUAL "debug")
  set(C_CXX_FLAGS "${C_CXX_FLAGS} -g3 -O0 -Wno-unused")
else()
  set(C_CXX_FLAGS "${C_CXX_FLAGS} -O3")
endif()

include_directories(
  inc
  src
)

# Set CPU architecture
set(CMAKE_C_FLAGS "${C_CXX_FLAGS} -D${ARCH}")
set(CMAKE_CXX_FLAGS "${C_CXX_FLAGS} -D${ARCH}")

# Common function for defining algorithm component
function(define_crypto_alg name namespace src inc test_src)
  add_library(
    pqclean_${name}
    OBJECT
    ${src}
  )

  target_include_directories(
    pqclean_${name} PRIVATE
    src/common
    ${inc}
  )

  target_compile_definitions(
    pqclean_${name} PRIVATE
    -DPQCLEAN_NAMESPACE=${namespace}
  )

  add_library(
    pqclean_test_${name}
    OBJECT
    ${test_src}
  )

  target_compile_definitions(
    pqclean_test_${name} PRIVATE
    -DPQCLEAN_NAMESPACE=${namespace}
  )

  target_include_directories(
    pqclean_test_${name} PRIVATE
    src/common
    ${inc}
  )

  add_executable(
    test_runner_${name}
  )
  target_link_libraries(
    test_runner_${name}

    common
    pqclean_${name}
    pqclean_test_${name}
  )
endfunction()

function(define_kem_alg name namespace src inc)
  define_crypto_alg(${name} ${namespace} "${src}" "${inc}" test/kem/testvectors.c)
endfunction()
function(define_sig_alg name namespace src inc)
  define_crypto_alg(${name} ${namespace} "${src}" "${inc}" test/sign/testvectors.c)
endfunction()

# Define sources of the components
set(
  SRC_CLEAN_DILITHIUM2
  src/sign/dilithium/dilithium2/clean/ntt.c
  src/sign/dilithium/dilithium2/clean/packing.c
  src/sign/dilithium/dilithium2/clean/poly.c
  src/sign/dilithium/dilithium2/clean/polyvec.c
  src/sign/dilithium/dilithium2/clean/reduce.c
  src/sign/dilithium/dilithium2/clean/rounding.c
  src/sign/dilithium/dilithium2/clean/sign.c
  src/sign/dilithium/dilithium2/clean/symmetric-shake.c
)

set(
  INC_CLEAN_DILITHIUM2
  src/sign/dilithium/dilithium2/clean
)
define_sig_alg(dilithium2_clean
  PQCLEAN_DILITHIUM2_CLEAN "${SRC_CLEAN_DILITHIUM2}" "${INC_CLEAN_DILITHIUM2}")

set(
  SRC_CLEAN_DILITHIUM3
  src/sign/dilithium/dilithium3/clean/ntt.c
  src/sign/dilithium/dilithium3/clean/packing.c
  src/sign/dilithium/dilithium3/clean/poly.c
  src/sign/dilithium/dilithium3/clean/polyvec.c
  src/sign/dilithium/dilithium3/clean/reduce.c
  src/sign/dilithium/dilithium3/clean/rounding.c
  src/sign/dilithium/dilithium3/clean/sign.c
  src/sign/dilithium/dilithium3/clean/symmetric-shake.c
)

set(
  INC_CLEAN_DILITHIUM3
  src/sign/dilithium/dilithium3/clean
)
define_sig_alg(dilithium3_clean
  PQCLEAN_DILITHIUM3_CLEAN "${SRC_CLEAN_DILITHIUM3}" "${INC_CLEAN_DILITHIUM3}")

set(
  SRC_CLEAN_DILITHIUM5
  src/sign/dilithium/dilithium5/clean/ntt.c
  src/sign/dilithium/dilithium5/clean/packing.c
  src/sign/dilithium/dilithium5/clean/poly.c
  src/sign/dilithium/dilithium5/clean/polyvec.c
  src/sign/dilithium/dilithium5/clean/reduce.c
  src/sign/dilithium/dilithium5/clean/rounding.c
  src/sign/dilithium/dilithium5/clean/sign.c
  src/sign/dilithium/dilithium5/clean/symmetric-shake.c
)

set(
  INC_CLEAN_DILITHIUM5
  src/sign/dilithium/dilithium5/clean
)

define_sig_alg(dilithium5_clean
  PQCLEAN_DILITHIUM5_CLEAN "${SRC_CLEAN_DILITHIUM5}" "${INC_CLEAN_DILITHIUM5}")

set(
  SRC_CLEAN_KYBER512
  src/kem/kyber/kyber512/clean/cbd.c
  src/kem/kyber/kyber512/clean/indcpa.c
  src/kem/kyber/kyber512/clean/kem.c
  src/kem/kyber/kyber512/clean/ntt.c
  src/kem/kyber/kyber512/clean/poly.c
  src/kem/kyber/kyber512/clean/polyvec.c
  src/kem/kyber/kyber512/clean/reduce.c
  src/kem/kyber/kyber512/clean/symmetric-shake.c
  src/kem/kyber/kyber512/clean/verify.c
)
set(
  INC_CLEAN_KYBER512
  src/kem/kyber/kyber512/clean
)
define_kem_alg(kyber512_clean
  PQCLEAN_KYBER512_CLEAN "${SRC_CLEAN_KYBER512}" "${INC_CLEAN_KYBER512}")

set(
  SRC_CLEAN_KYBER768
  src/kem/kyber/kyber768/clean/cbd.c
  src/kem/kyber/kyber768/clean/indcpa.c
  src/kem/kyber/kyber768/clean/kem.c
  src/kem/kyber/kyber768/clean/ntt.c
  src/kem/kyber/kyber768/clean/poly.c
  src/kem/kyber/kyber768/clean/polyvec.c
  src/kem/kyber/kyber768/clean/reduce.c
  src/kem/kyber/kyber768/clean/symmetric-shake.c
  src/kem/kyber/kyber768/clean/verify.c
)
set(
  INC_CLEAN_KYBER768
  src/kem/kyber/kyber768/clean
)
define_kem_alg(kyber768_clean
  PQCLEAN_KYBER768_CLEAN "${SRC_CLEAN_KYBER768}" "${INC_CLEAN_KYBER768}")

set(
  SRC_CLEAN_KYBER1024
  src/kem/kyber/kyber1024/clean/cbd.c
  src/kem/kyber/kyber1024/clean/indcpa.c
  src/kem/kyber/kyber1024/clean/kem.c
  src/kem/kyber/kyber1024/clean/ntt.c
  src/kem/kyber/kyber1024/clean/poly.c
  src/kem/kyber/kyber1024/clean/polyvec.c
  src/kem/kyber/kyber1024/clean/reduce.c
  src/kem/kyber/kyber1024/clean/symmetric-shake.c
  src/kem/kyber/kyber1024/clean/verify.c
)
set(
  INC_CLEAN_KYBER1024
  src/kem/kyber/kyber1024/clean
)
define_kem_alg(kyber1024_clean
  PQCLEAN_KYBER1024_CLEAN "${SRC_CLEAN_KYBER1024}" "${INC_CLEAN_KYBER1024}")

set(
  SRC_CLEAN_SABER
  src/kem/saber/saber/clean/cbd.c
  src/kem/saber/saber/clean/kem.c
  src/kem/saber/saber/clean/pack_unpack.c
  src/kem/saber/saber/clean/poly.c
  src/kem/saber/saber/clean/poly_mul.c
  src/kem/saber/saber/clean/SABER_indcpa.c
  src/kem/saber/saber/clean/verify.c)
set(
  INC_CLEAN_SABER
  src/kem/saber/saber/clean)
define_kem_alg(
  saber_clean
  PQCLEAN_SABER_CLEAN "${SRC_CLEAN_SABER}" "${INC_CLEAN_SABER}")

set(
  SRC_CLEAN_FIRESABER
  src/kem/saber/firesaber/clean/cbd.c
  src/kem/saber/firesaber/clean/kem.c
  src/kem/saber/firesaber/clean/pack_unpack.c
  src/kem/saber/firesaber/clean/poly.c
  src/kem/saber/firesaber/clean/poly_mul.c
  src/kem/saber/firesaber/clean/SABER_indcpa.c
  src/kem/saber/firesaber/clean/verify.c)
set(
  INC_CLEAN_FIRESABER
  src/kem/saber/firesaber/clean)
define_kem_alg(
  firesaber_clean
  PQCLEAN_FIRESABER_CLEAN "${SRC_CLEAN_FIRESABER}" "${INC_CLEAN_FIRESABER}")

set(
  SRC_CLEAN_LIGHTSABER
  src/kem/saber/lightsaber/clean/cbd.c
  src/kem/saber/lightsaber/clean/kem.c
  src/kem/saber/lightsaber/clean/pack_unpack.c
  src/kem/saber/lightsaber/clean/poly.c
  src/kem/saber/lightsaber/clean/poly_mul.c
  src/kem/saber/lightsaber/clean/SABER_indcpa.c
  src/kem/saber/lightsaber/clean/verify.c)
set(
  INC_CLEAN_LIGHTSABER
  src/kem/saber/lightsaber/clean)
define_kem_alg(
  lightsaber_clean
  PQCLEAN_LIGHTSABER_CLEAN "${SRC_CLEAN_LIGHTSABER}" "${INC_CLEAN_LIGHTSABER}")

# AVX2 targets
if(${ARCH} STREQUAL "ARCH_x86_64")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native")

set(
  SRC_AVX2_SABER
  src/kem/saber/saber/avx2/cbd.c
  src/kem/saber/saber/avx2/kem.c
  src/kem/saber/saber/avx2/pack_unpack.c
  src/kem/saber/saber/avx2/poly.c
  src/kem/saber/saber/avx2/poly_mul.c
  src/kem/saber/saber/avx2/SABER_indcpa.c
  src/kem/saber/saber/avx2/verify.c)
set(
  INC_AVX2_SABER
  src/kem/saber/saber/avx2)
define_kem_alg(
  saber_avx2
  PQCLEAN_SABER_AVX2 "${SRC_AVX2_SABER}" "${INC_AVX2_SABER}")

set(
  SRC_AVX2_FIRESABER
  src/kem/saber/firesaber/avx2/cbd.c
  src/kem/saber/firesaber/avx2/kem.c
  src/kem/saber/firesaber/avx2/pack_unpack.c
  src/kem/saber/firesaber/avx2/poly.c
  src/kem/saber/firesaber/avx2/poly_mul.c
  src/kem/saber/firesaber/avx2/SABER_indcpa.c
  src/kem/saber/firesaber/avx2/verify.c)
set(
  INC_AVX2_FIRESABER
  src/kem/saber/firesaber/avx2)
define_kem_alg(
  firesaber_avx2
  PQCLEAN_FIRESABER_AVX2 "${SRC_AVX2_FIRESABER}" "${INC_AVX2_FIRESABER}")

set(
  SRC_AVX2_LIGHTSABER
  src/kem/saber/lightsaber/avx2/cbd.c
  src/kem/saber/lightsaber/avx2/kem.c
  src/kem/saber/lightsaber/avx2/pack_unpack.c
  src/kem/saber/lightsaber/avx2/poly.c
  src/kem/saber/lightsaber/avx2/poly_mul.c
  src/kem/saber/lightsaber/avx2/SABER_indcpa.c
  src/kem/saber/lightsaber/avx2/verify.c)
set(
  INC_AVX2_LIGHTSABER
  src/kem/saber/lightsaber/avx2)
define_kem_alg(
  lightsaber_avx2
  PQCLEAN_LIGHTSABER_AVX2 "${SRC_AVX2_LIGHTSABER}" "${INC_AVX2_LIGHTSABER}")

set(
  SRC_AVX2_DILITHIUM2
  src/sign/dilithium/dilithium2/avx2/consts.c
  src/sign/dilithium/dilithium2/avx2/f1600x4.S
  src/sign/dilithium/dilithium2/avx2/fips202x4.c
  src/sign/dilithium/dilithium2/avx2/invntt.S
  src/sign/dilithium/dilithium2/avx2/ntt.S
  src/sign/dilithium/dilithium2/avx2/packing.c
  src/sign/dilithium/dilithium2/avx2/pointwise.S
  src/sign/dilithium/dilithium2/avx2/poly.c
  src/sign/dilithium/dilithium2/avx2/polyvec.c
  src/sign/dilithium/dilithium2/avx2/rejsample.c
  src/sign/dilithium/dilithium2/avx2/rounding.c
  src/sign/dilithium/dilithium2/avx2/shuffle.S
  src/sign/dilithium/dilithium2/avx2/sign.c
  src/sign/dilithium/dilithium2/avx2/symmetric-shake.c)
set(
  INC_AVX2_DILITHIUM2
  src/sign/dilithium/dilithium2/avx2
)
define_sig_alg(dilithium2_avx2
  PQCLEAN_DILITHIUM2_AVX2 "${SRC_AVX2_DILITHIUM2}" "${INC_AVX2_DILITHIUM2}")

set(
  SRC_AVX2_DILITHIUM3
  src/sign/dilithium/dilithium3/avx2/consts.c
  src/sign/dilithium/dilithium3/avx2/f1600x4.S
  src/sign/dilithium/dilithium3/avx2/fips202x4.c
  src/sign/dilithium/dilithium3/avx2/invntt.S
  src/sign/dilithium/dilithium3/avx2/ntt.S
  src/sign/dilithium/dilithium3/avx2/packing.c
  src/sign/dilithium/dilithium3/avx2/pointwise.S
  src/sign/dilithium/dilithium3/avx2/poly.c
  src/sign/dilithium/dilithium3/avx2/polyvec.c
  src/sign/dilithium/dilithium3/avx2/rejsample.c
  src/sign/dilithium/dilithium3/avx2/rounding.c
  src/sign/dilithium/dilithium3/avx2/shuffle.S
  src/sign/dilithium/dilithium3/avx2/sign.c
  src/sign/dilithium/dilithium3/avx2/symmetric-shake.c)
set(
  INC_AVX2_DILITHIUM3
  src/sign/dilithium/dilithium3/avx2
)
define_sig_alg(dilithium3_avx2
  PQCLEAN_DILITHIUM3_AVX2 "${SRC_AVX2_DILITHIUM3}" "${INC_AVX2_DILITHIUM3}")

set(
  SRC_AVX2_DILITHIUM5
  src/sign/dilithium/dilithium5/avx2/consts.c
  src/sign/dilithium/dilithium5/avx2/f1600x4.S
  src/sign/dilithium/dilithium5/avx2/fips202x4.c
  src/sign/dilithium/dilithium5/avx2/invntt.S
  src/sign/dilithium/dilithium5/avx2/ntt.S
  src/sign/dilithium/dilithium5/avx2/packing.c
  src/sign/dilithium/dilithium5/avx2/pointwise.S
  src/sign/dilithium/dilithium5/avx2/poly.c
  src/sign/dilithium/dilithium5/avx2/polyvec.c
  src/sign/dilithium/dilithium5/avx2/rejsample.c
  src/sign/dilithium/dilithium5/avx2/rounding.c
  src/sign/dilithium/dilithium5/avx2/shuffle.S
  src/sign/dilithium/dilithium5/avx2/sign.c
  src/sign/dilithium/dilithium5/avx2/symmetric-shake.c)
set(
  INC_AVX2_DILITHIUM5
  src/sign/dilithium/dilithium5/avx2
)
define_sig_alg(dilithium5_avx2
  PQCLEAN_DILITHIUM5_AVX2 "${SRC_AVX2_DILITHIUM5}" "${INC_AVX2_DILITHIUM5}")

set(
  SRC_AVX2_KYBER512
  src/kem/kyber/kyber512/avx2/cbd.c
  src/kem/kyber/kyber512/avx2/consts.c
  src/kem/kyber/kyber512/avx2/fips202x4.c
  src/kem/kyber/kyber512/avx2/indcpa.c
  src/kem/kyber/kyber512/avx2/kem.c
  src/kem/kyber/kyber512/avx2/poly.c
  src/kem/kyber/kyber512/avx2/polyvec.c
  src/kem/kyber/kyber512/avx2/rejsample.c
  src/kem/kyber/kyber512/avx2/symmetric-shake.c
  src/kem/kyber/kyber512/avx2/verify.c
  src/kem/kyber/kyber512/avx2/basemul.S
  src/kem/kyber/kyber512/avx2/fq.S
  src/kem/kyber/kyber512/avx2/invntt.S
  src/kem/kyber/kyber512/avx2/ntt.S
  src/kem/kyber/kyber512/avx2/shuffle.S
)
set(
  INC_AVX2_KYBER512
  src/kem/kyber/kyber512/avx2
)
define_kem_alg(kyber512_avx2
  PQCLEAN_KYBER512_AVX2 "${SRC_AVX2_KYBER512}" "${INC_AVX2_KYBER512}")

set(
  SRC_AVX2_KYBER768
  src/kem/kyber/kyber768/avx2/cbd.c
  src/kem/kyber/kyber768/avx2/consts.c
  src/kem/kyber/kyber768/avx2/fips202x4.c
  src/kem/kyber/kyber768/avx2/indcpa.c
  src/kem/kyber/kyber768/avx2/kem.c
  src/kem/kyber/kyber768/avx2/poly.c
  src/kem/kyber/kyber768/avx2/polyvec.c
  src/kem/kyber/kyber768/avx2/rejsample.c
  src/kem/kyber/kyber768/avx2/symmetric-shake.c
  src/kem/kyber/kyber768/avx2/verify.c
  src/kem/kyber/kyber768/avx2/basemul.S
  src/kem/kyber/kyber768/avx2/fq.S
  src/kem/kyber/kyber768/avx2/invntt.S
  src/kem/kyber/kyber768/avx2/ntt.S
  src/kem/kyber/kyber768/avx2/shuffle.S
)
set(
  INC_AVX2_KYBER768
  src/kem/kyber/kyber768/avx2
)
define_kem_alg(kyber768_avx2
  PQCLEAN_KYBER768_AVX2 "${SRC_AVX2_KYBER768}" "${INC_AVX2_KYBER768}")

set(
  SRC_AVX2_KYBER1024
  src/kem/kyber/kyber1024/avx2/cbd.c
  src/kem/kyber/kyber1024/avx2/consts.c
  src/kem/kyber/kyber1024/avx2/fips202x4.c
  src/kem/kyber/kyber1024/avx2/indcpa.c
  src/kem/kyber/kyber1024/avx2/kem.c
  src/kem/kyber/kyber1024/avx2/poly.c
  src/kem/kyber/kyber1024/avx2/polyvec.c
  src/kem/kyber/kyber1024/avx2/rejsample.c
  src/kem/kyber/kyber1024/avx2/symmetric-shake.c
  src/kem/kyber/kyber1024/avx2/verify.c
  src/kem/kyber/kyber1024/avx2/basemul.S
  src/kem/kyber/kyber1024/avx2/fq.S
  src/kem/kyber/kyber1024/avx2/invntt.S
  src/kem/kyber/kyber1024/avx2/ntt.S
  src/kem/kyber/kyber1024/avx2/shuffle.S
)
set(
  INC_AVX2_KYBER1024
  src/kem/kyber/kyber1024/avx2
)
define_kem_alg(kyber1024_avx2
  PQCLEAN_KYBER1024_AVX2 "${SRC_AVX2_KYBER1024}" "${INC_AVX2_KYBER1024}")
endif()

# The rest of the library
set(SRC_COMMON_GENERIC
  src/common/fips202.c
  src/common/sp800-185.c
  src/common/randombytes.c
)

if(${ARCH} STREQUAL "ARCH_x86_64")
set(SRC_COMMON_AVX2
  src/common/keccak4x/KeccakP-1600-times4-SIMD256.c
)
endif()

add_library(
  common
  OBJECT
  ${SRC_COMMON_GENERIC}
  ${SRC_COMMON_AVX2}
)

add_library(
  pqclean
  SHARED
)

add_library(
  pqclean_s
  STATIC
)

target_link_libraries(
  pqclean
  common
  pqclean_dilithium2_clean
  pqclean_dilithium3_clean
  pqclean_dilithium5_clean
)

target_link_libraries(
  pqclean_s
  common
  pqclean_dilithium2_clean
  pqclean_dilithium3_clean
  pqclean_dilithium5_clean
)

# TODO: this requires changes to testvectors.c
# add_executable(
#   test
# )
#
# target_link_libraries(
#   test
#   pqclean_s
#   pqclean_test_dilithium2_clean
#   pqclean_test_dilithium3_clean
#   pqclean_test_dilithium5_clean
# )
