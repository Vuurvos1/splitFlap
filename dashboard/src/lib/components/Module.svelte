<script lang="ts">
	interface Props {
		character: string;
		selected: boolean;
		onSelect?: () => void;
	}

	let {
		character = $bindable(''),
		selected = $bindable(false),
		onSelect = $bindable(() => {})
	}: Props = $props();

	let inputRef = $state<HTMLInputElement | null>(null);

	$effect(() => {
		if (inputRef && selected) {
			inputRef.focus();
		}
	});
</script>

<div class="module" class:selected class:bg-stone-600!={selected}>
	<div class="module-content">
		<div
			class="flaps"
			style:background-color={character === '|' ? 'white' : 'var(--module-color)'}
		></div>
		<div class="flap-gap"></div>

		<input
			bind:this={inputRef}
			class="flap-input"
			style:color={character === '|' ? 'transparent' : 'white'}
			maxlength="1"
			type="text"
			bind:value={character}
			onclick={onSelect}
		/>

		<div class="top-shadow"></div>
		<div class="bottom-shadow"></div>
	</div>
</div>

<style>
	.module {
		--module-color: #44403c;
		--shadow-color: #0c0a09;

		background-color: var(--module-color);

		padding: 3rem 1.5rem 2rem 1.5rem;

		color: #fafafa;
		font-family: 'Fira Mono', monospace;
		font-weight: 600;

		/* outline: 2px solid red; */

		/* border-radius: 0.5rem;
		padding: 0.5rem;
		box-shadow: 0 0 0.5rem 0.5rem var(--shadow-color); */
	}

	/* .module.selected {
	} */

	.module-content {
		background-color: var(--shadow-color);
		position: relative;
	}

	.flaps {
		position: absolute;
		inset: 4px;
		background-color: var(--module-color);
	}

	.flap-gap {
		position: absolute;
		pointer-events: none;
		z-index: 1;

		height: 0.25rem;
		left: 0;
		right: 0;
		top: 40%;
		background-color: var(--shadow-color);
	}

	.top-shadow,
	.bottom-shadow {
		pointer-events: none;
	}

	.top-shadow {
		position: absolute;
		top: 0;
		left: 0;
		right: 0;
		height: 1rem;
		background: linear-gradient(to bottom, rgb(0 0 0 / 1), transparent);
	}

	.bottom-shadow {
		position: absolute;
		bottom: 0;
		left: 0;
		right: 0;
		height: 3.5rem;
		background: linear-gradient(to top, rgb(0 0 0 / 1), transparent);
	}

	.flap-input {
		position: relative;
		text-align: center;
		font-size: 8rem;
		width: 1.5ch;
		min-width: 0;
		background-color: transparent;
		color: inherit;
		margin-bottom: 2rem;

		&:focus {
			outline: none;
		}
	}
</style>
