__kernel void convolve(
    const __global uint * const input,
    __constant uint * const mask,
    __global uint * const output,
    const int input_width,
    const int mask_width) {
    const int x = get_global_id(0);
    const int y = get_global_id(1);

    uint sum = 0;
    for (int r = 0; r < mask_width; r++) {
        const int idx_int_mp = (y + r) * input_width + x;
        for (int c = 0; c < mask_width; c++) {
            sum += mask[(r * mask_width) + c] * input[idx_int_mp + c];
        }
    }
    output[y * get_global_size(0) + x] = sum;
}