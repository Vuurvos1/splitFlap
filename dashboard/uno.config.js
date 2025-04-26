import extractorSvelte from '@unocss/extractor-svelte';
import presetWind3 from '@unocss/preset-wind3';
import { defineConfig } from 'unocss';

export default defineConfig({
	extractors: [extractorSvelte()],
	shortcuts: [],
	presets: [presetWind3()]
});
