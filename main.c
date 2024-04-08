#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

// Spinning formula: y^2 = (-x^2 - (1.2z + |x| * (2/3)) +1^2) / (2 -z)^2
// REAL FORMULA:  (x^{2}+2.25y^{2}+z^{2}-1)^{3}-x^{2}z^{3}-0.05y^{2}z^{3}=0

#define SLEEP 3000
#define ROWS 100
#define COLS 40
#define XY_START -.5f
#define XY_END .5f

int main(void)
{
	// Clear screen hide cursor
	printf("\x1b[2J\x1b[?5l");

	float loopCounter = 0;

	while (true)
	{
		const float scaleFactorCos = cos(loopCounter);
		const float scaleFactorSin = sin(loopCounter);

		float grid2D[ROWS * COLS] = {0};
		float maxZVal = 0;

		for (float y = -0.5f; y <= 0.5f; y += 0.01f)
		{
			///  INFO: This `scale` value will be dinamicaly setted based on the `scaleFactorCos`, `scaleFactorSin` and `loopCounter` variables

			const float scale = 0.4f + 0.2f * pow(0.2f + 0.2f * sin(loopCounter * 6 + y * 2), 2);

			for (float x = -0.5f; x <= 0.5f; x += 0.01f)
			{
				float z = -x * x - pow(1.2f * y - fabs(x) * 2 / 3, 2) + pow(scale, 2);  // 3D Heart Shape Formula

				// I really don't get it, what is this condition important?

				// if (z < 0)
				// {
				//	continue;
				// }

				z = sqrt(z) / (2 - y);

				///  INFO: This `tmpZ` variable is just used to generate an 2D representation of the *xyz* values

				for (float tmpZ = -z; tmpZ <= z; tmpZ += z / 6)
				{
					float rotatedX = x * scaleFactorCos - tmpZ * scaleFactorSin;
					float rotatedZ = x * scaleFactorSin + tmpZ * scaleFactorCos;

					float perspective = 1 + rotatedZ / 2;

					uint8_t xGridPos = lroundf((rotatedX * perspective + 0.5f) * 80);
					uint8_t yGridPos = lroundf((-y * perspective + 0.5f) * 39);

					int index = xGridPos + yGridPos * ROWS;

					if (grid2D[index] <= rotatedZ)
					{
						grid2D[index] = rotatedZ;

						if (maxZVal <= rotatedZ)
						{
							maxZVal = rotatedZ;
						}
					}
				}
			}
		}

		printf("\x1b[H");

		for (int i = 0; i < ROWS * COLS; ++i)
		{
			putchar(i % ROWS ? " iiiiii#####@@"[lroundf(grid2D[i] / maxZVal * 13)] : '\n');
		}

		loopCounter += 0.003f;

		usleep(SLEEP);
	}

	return 0;
}
