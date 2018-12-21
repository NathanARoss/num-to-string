# num-to-string
Test bed for converting numeric types to character arrays

I am finding small efficient implementations of ltoa and dtoa for use in WebAssembly.  I do not want to define log10 or pow10 inside Wasm or call a JS function when casting doubles to strings in WebAssembly.
