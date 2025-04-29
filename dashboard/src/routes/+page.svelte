<script lang="ts">
	import Module from '$lib/components/Module.svelte';

	const rows = 2;
	const columns = 8;

	let selectedCell = $state({ row: 0, column: 0 });

	function createBoard(rows: number, columns: number) {
		return Array.from({ length: rows }, () => Array(columns).fill(' '));
	}
	let board = $state(createBoard(rows, columns));

	function setRow(row: number, characters: string) {
		const chars = characters.split('');
		board[row] = board[row].map((_, i) => chars[i] || '');
	}

	setRow(0, 'HELLO');
	setRow(1, 'WORLD');

	const characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ23456789 |?!+-=*':,.@#%$"; // €

	function clamp(min: number, max: number, value: number) {
		return Math.max(min, Math.min(max, value));
	}

	function clampSelectedCell() {
		selectedCell.row = clamp(0, rows - 1, selectedCell.row);
		selectedCell.column = clamp(0, columns - 1, selectedCell.column);
	}

	function handleKeyDown(event: KeyboardEvent) {
		console.log(event.key);
		event.preventDefault();

		if (event.key === 'Enter') {
			selectedCell.row++;
			selectedCell.column = 0;
		}

		if (event.key === 'Backspace') {
			board[selectedCell.row][selectedCell.column] = '';
			selectedCell.column--;
		}

		//delete
		if (event.key === 'Delete') {
			board[selectedCell.row][selectedCell.column] = '';
		}

		if (event.key === 'ArrowUp') {
			selectedCell.row--;
		}

		if (event.key === 'ArrowDown') {
			selectedCell.row++;
		}

		if (event.key === 'ArrowRight') {
			selectedCell.column++;
		}

		if (event.key === 'ArrowLeft') {
			selectedCell.column--;
		}

		if (event.shiftKey && event.key === 'Tab') {
			selectedCell.column--;
		}

		if (!event.shiftKey && event.key === 'Tab') {
			selectedCell.column++;
		}

		// check if the character is valid
		if (!characters.includes(event.key.toUpperCase())) {
			clampSelectedCell();

			return;
		}

		board[selectedCell.row][selectedCell.column] = event.key.toUpperCase();
		selectedCell.column++;

		// TODO: handle "dead" keys

		clampSelectedCell();
	}

	function saveSplitflap() {
		// turn characters into index array
		const indexArray = board
			.map((row) => row.map((character) => characters.indexOf(character || ' ')))
			.flat();

		fetch('/api/splitflap', {
			method: 'POST',
			body: JSON.stringify(indexArray)
		});
	}
</script>

<main class="h-full flex flex-col">
	<div class="flex flex-row items-center justify-between gap-4 py-4 px-2">
		<h1 class="text-4xl font-medium leading-[1.5]">Configure splitflap</h1>

		<button class="rounded-md bg-blue-500 px-4 py-2 text-white" onclick={saveSplitflap}>Save</button
		>
	</div>

	<section class="bg-gray-200 px-8 flex-1 flex flex-col items-center justify-center">
		<!-- svelte-ignore a11y_no_static_element_interactions -->
		<div
			class="grid w-fit"
			style="grid-template-columns: repeat({columns}, 1fr); grid-template-rows: repeat({rows}, 1fr);"
			aria-label="Split Flap Board"
			onkeydown={handleKeyDown}
		>
			{#each board as row, rowIndex}
				{#each row as character, columnIndex}
					<Module
						{character}
						selected={selectedCell.row === rowIndex && selectedCell.column === columnIndex}
						onSelect={() => {
							selectedCell.row = rowIndex;
							selectedCell.column = columnIndex;
						}}
					/>
				{/each}
			{/each}
		</div>
	</section>
</main>
