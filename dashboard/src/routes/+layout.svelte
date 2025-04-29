<script lang="ts">
	import '@unocss/reset/tailwind.css';
	import 'uno.css';

	import '../app.css';

	let { children } = $props();
</script>

{@render children()}
