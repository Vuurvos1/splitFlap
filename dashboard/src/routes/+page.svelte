<script lang="ts">
	import Module from '$lib/components/Module.svelte';

	const rows = 4;
	const columns = 8;

	function createBoard(rows: number, columns: number) {
		return Array.from({ length: rows }, () => Array(columns).fill(''));
	}
	let board = $state(createBoard(rows, columns));

	function setRow(row: number, characters: string) {
		const chars = characters.split('');
		board[row] = board[row].map((_, i) => chars[i] || '');
	}

	setRow(0, 'HELLO');
	setRow(1, 'WORLD');

	const characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ23456789 |?!+-=*':,.@#%$"; // €
	console.log(characters.length);

	function handleKeyDown(event: KeyboardEvent) {
		console.log(event.key);

		// TODO: handle arrows
	}
</script>

<!-- svelte-ignore a11y_no_static_element_interactions -->
<div
	class="grid w-fit"
	style="grid-template-columns: repeat({columns}, 1fr); grid-template-rows: repeat({rows}, 1fr);"
	aria-label="Split Flap Board"
	onkeydown={handleKeyDown}
>
	{#each board as row}
		{#each row as character}
			<Module {character} />
		{/each}
	{/each}
</div>
