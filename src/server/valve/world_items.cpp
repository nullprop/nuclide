/*
 * Copyright (c) 2016-2020 Marco Hladik <marco@icculus.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* This is one of those leftovers from trying to get a game out in time */
class world_items:CBaseTrigger
{
	void() world_items;
};

void world_items::world_items(void)
{
	int nfields = tokenize(__fullspawndata);
	for (int i = 1; i < (nfields - 1); i += 2) {
		switch (argv(i)) {
		case "type":
			float type = stof(argv(i+1));
			switch (type) {
			case 44:
				spawnfunc_item_battery();
				break;
			case 45:
				spawnfunc_item_suit();
				break;
			default:
			}
		default:
			break;
		}
	}
}
