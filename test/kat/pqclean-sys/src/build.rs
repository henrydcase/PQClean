extern crate cmake;
use cmake::Config;
extern crate bindgen;

fn main() {
	let dst = Config::new("../../../")
		.profile("Release")
		.very_verbose(true)
		.build();

	println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=dylib=pqclean");
    println!("cargo:rerun-if-changed=../../../src/sign/dilithium/dilithium2/clean/*");

    // The bindgen::Builder is the main entry point
    // to bindgen, and lets you build up options for
    // the resulting bindings.
    let bindings = bindgen::Builder::default()
        // The input header we would like to generate
        // bindings for.
        .header("../../../src/sign/dilithium/dilithium2/clean/api.h")
        // Point to PQClean include file
        .clang_arg("-I../../../src/sign/dilithium/dilithium2/clean")
        // Tell cargo to invalidate the built crate whenever any of the
        // included header files changed.
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        // Add "Default" whenever possible
        .derive_default(true)
        // Finish the builder and generate the bindings.
        .generate()
        // Unwrap the Result and panic on failure.
        .expect("Unable to generate bindings");
    bindings
    	.write_to_file("src/bindings.rs")
    	.expect("Couldn't write bindings");
}
