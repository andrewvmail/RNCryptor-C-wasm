import rncryptor from "./rncryptor"

export default class Rncryptor {
	constructor(api) {
		this.api = api;
	}

	static async initialize({ rncryptorWasmLocation }) {
		try {
			const Module = await rncryptor({
				locateFile: (f) => {
					return rncryptorWasmLocation;
				},
			})
			const api = {
				FS: Module.FS,
				encrypt: Module.cwrap(
					"encrypt",
					"",
					["string", "string", "string"],
					),
				decrypt: Module.cwrap(
					"decrypt",
					"",
					["string", "string", "string"],
					),
			};
			return new Rncryptor(api);
		} catch (error) {
			throw new Error(`Failed to fetch data: ${error}`);
		}
	}
}