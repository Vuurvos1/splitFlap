import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const filename = fileURLToPath(import.meta.url);
const dirname = path.dirname(filename);

const buildDir = path.join(dirname, 'build');
const controllerDir = path.join(dirname, '../code/controller/data');

try {
	fs.cpSync(buildDir, controllerDir, { recursive: true });
	console.log(`Copied build directory from ${buildDir} to ${controllerDir}`);
} catch (err) {
	console.error('Error copying build directory:', err);
}
