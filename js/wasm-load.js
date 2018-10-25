function wasm_instantiate(buf, opt) {
        const mod = new WebAssembly.Module(buf)
        return (new WebAssembly.Instance(mod, opt)).exports;
}

function wasm_load(path) {
#if defined(NODE) && defined(OLD)
        return new Promise(resolve => {
                const buf = fs.readFileSync(path);
                resolve(buf);
        });
#else
        return fetch(path)
                .then(res => res.arrayBuffer());
#endif
}

function wasm_load_inst(path, env) {
        return wasm_load(path).then(buf => wasm_instantiate(buf, { env }));
}
