// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <cstdint>
#include <malloc.h>

extern "C" __declspec(dllexport) void Filter(double* image_data, double* image_kernel, int32_t rows, int32_t columns, int32_t kernel_dim, int32_t kernel_div, int32_t div);

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void Filter(double* image_data, double* image_kernel, int32_t rows, int32_t columns, int32_t kernel_dim, int32_t kernel_div, int32_t div) {
	int correction = 0;

	if (kernel_dim % 2 == 0) {
		correction = 1;
	}

	double* result = (double*) malloc ((rows + 2 * kernel_div) * (columns + 2 * kernel_div) * sizeof(double));
	for (int i = 0; i < (rows + 2 * kernel_div) * (columns + 2 * kernel_div); i++) {
		result[i] = 0;
	}

	double* temp_image = (double *) malloc (kernel_dim * kernel_dim * sizeof(double));
	for (int i = 0; i < kernel_dim * kernel_dim; i++) {
		temp_image[i] = 0;
	}

	for (int i = kernel_div; i < rows + kernel_div; i++) {
		for (int j = kernel_div; j < columns + kernel_div; j++) {
			int k = 0;
			for (int x = i - kernel_div; x < i + kernel_div - correction + 1; x++) {
				for (int y = j - kernel_div; y < j + kernel_div - correction + 1; y++) {
					temp_image[k++] = image_data[x * (rows + 2 * kernel_div) + y];
				}
			}
			double sum = 0;
			for (int l = 0; l < kernel_dim * kernel_dim; l++) {
				sum += temp_image[l] * image_kernel[l];
			}
			result[i * (rows + 2 * kernel_div) + j] = sum / div;
		}
	}

	for (int i = 0; i < (rows + 2 * kernel_div) * (columns + 2 * kernel_div); i++) {
		image_data[i] = result[i];
	}

	free(temp_image);
	free(result);

	return;
}