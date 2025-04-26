<script lang="ts">
	import '@unocss/reset/tailwind.css';
	import 'uno.css';

	import '../app.css';
	import '@fontsource/fira-mono';

	let { children } = $props();
</script>

{@render children()}
