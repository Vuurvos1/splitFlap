<script lang="ts">
	import Module from '$lib/components/module.svelte';

	const words = 'HELLO WORLD';
	const characters = words.split('');
</script>

<div class="flex flex-row">
	{#each characters as character}
		<Module {character} />
	{/each}
</div>
<div class="flex flex-row">
	{#each characters as character}
		<Module {character} />
	{/each}
</div>
