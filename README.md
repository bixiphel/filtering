# Image Filtering Project — Gaussian and Bilateral Filters

## Overview

This project implements two common image smoothing techniques in C:

* Gaussian filtering
* Bilateral filtering

Both programs operate on **grayscale PGM images (specifically the P5 format)** and were written as part of an image processing assignment. The filters are implemented manually without using any third-party image processing libraries.

---

## What the Programs Do

Each program:

1. Reads in a grayscale `.pgm` image file.
2. Applies a specified image filter.
3. Writes the filtered result to a new `.pgm` file.

The filters use *zero-padding*, meaning pixels outside the image boundary are treated as having intensity value 0.

---

## Gaussian Filter

### Purpose

The Gaussian filter performs **uniform smoothing** to reduce noise and blur an image. Specifically, this program implements 1D Gaussian filtering, or Gaussian filtering in a single direction.

### How It Works

* A Gaussian kernel is generated using a user-specified:

  * Kernel size (must be odd)
  * Sigma value (controls blur strength)
* Each pixel is replaced by a weighted average of its neighbors.
* Weights depend only on **spatial distance** from the center pixel.

### Effect on Images

Gaussian filtering:

* Smooths noise
* Blurs edges
* Produces a soft, uniform blur across the image

---

## Bilateral Filter

### Purpose

The bilateral filter smooths images while **preserving edges**.

### How It Works

Unlike Gaussian filtering, bilateral filtering uses two weighting factors:

1. **Spatial weight** — depends on pixel distance (like Gaussian).
2. **Intensity weight** — depends on intensity difference between pixels.

The final weight is the product of these two factors, so pixels with very different intensities (such as across an edge) contribute less to the average.

### Effect on Images

Bilateral filtering:

* Reduces noise within regions
* Preserves sharp edges
* Prevents blurring across intensity boundaries

---

## Example Output Comparison

| Original Image | Gaussian Filter (kernel = 5x5, σ = 2.1) | Bilateral Filter (kernel = 5x5, σ_s = σ_i = 32000) |
| -------------- | --------------------------------------- | -------------------------------------------------- |
| <img src="https://github.com/bixiphel/filtering/blob/19d2c3cf2f1953b76f9afae2d31f3156a05d5953/example%20outputs/test-img.png" /> | <img src="https://github.com/bixiphel/filtering/blob/0298b236e47ec9199441679dd7b83da08b20cab8/example%20outputs/output_gauss.png" /> | <img src="https://github.com/bixiphel/filtering/blob/0298b236e47ec9199441679dd7b83da08b20cab8/example%20outputs/output_bi.png" /> | 

---

## Compilation

Both programs require linking with the math library (this is due to using the `exp` and `sqrt` functions, which aren't natively included in the `gcc` command):

```bash
gcc gaussian.c -o gaussian -lm
gcc bilateral.c -o bilateral -lm
```

---

## Example Usage

### Gaussian Filter

General instruction:
```bash
./gaussian inputFilename.pgm outputFilename.pgm kernel_size sigma
```

Example:

```bash
./gaussian image.pgm gaussian_out.pgm 5 2.1
```


#### Notes
* `kernel_size` must be an odd positive integer (i.e. `3`, `5`, `131`)
* sigma must be larger than 0; it does not necessarily have to be an integer (i.e. can be double or floating-point)
---

### Bilateral Filter

General instruction:
```bash
./bilateral inputFilename.pgm outputFilename.pgm kernel_size spatial_sigma intensity_sigma 
```

Example:

```bash
./bilateral image.pgm bilateral_out.pgm 5 2.0 30.0
```

#### Notes
* `kernel_size` must be an odd positive integer (i.e. `3`, `5`, `131`)
* spatial_sigma **AND** intensity_sigma must **both** be larger than 0; neither necessarily has to be an integer (i.e. can be double or floating-point)

---
