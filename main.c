#include <math.h>
#include <unistd.h>
#include <stdio.h>

// Spinning formula: y^2 = (-x^2 - (1.2z + |x| * (2/3)) +1^2) / (2 -z)^2

int main(void)
{
	// Clear screen hide cursor
	printf("\x1b[2J\x1b[?5l");

	float t = 0;

	while (1)
	{
		float zb[100 * 40] = {0};
		float maxz = 0, c = cos(t), s = sin(t);

		for (float y = -0.5f; y <= 0.5f; y += 0.01f)
		{
			for (float x = -0.5f; x <= 0.5f; x += 0.01f)
			{
				// heart formula

				float z = -x * x - pow(1.2f * y - fabs(x) * 2 / 3, 2) + 0.1;

				if (z < 0)
				{
					continue;
				}

				z = sqrt(z) / (2 - y);

				for (float tz = -z; tz <= z; tz += z / 6)
				{
					// Rotate

					float nx = x * c - tz * s;
					float nz = x * s + tz * c;

					// Add perspective

					float p = 1 + nz / 2;

					int vx = lroundf((nx * p + 0.5f) * 80);
					int vy = lroundf((-y * p + 0.5f) * 39);

					int idx = vx + vy * 100;

					if (zb[idx] <= nz)
					{
						zb[idx] = nz;

						if (maxz <= nz)
						{
							maxz = nz;
						}
					}
				}
			}
		}

		printf("\x1b[H");

		for (int i = 0; i < 100 * 40; i++)
		{
			putchar(i % 100 ? " .,-~:;=!*###@"[lroundf(zb[i] / maxz * 13)] : 10);
		}

		t += 0.003f;

		usleep(3000);
	}

	return 0;
}
