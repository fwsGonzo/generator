float swissTurbulence(float2 p, float seed, int octaves,
                      float lacunarity = 2.0, float gain = 0.5,
                      float warp = 0.15)
{
     float sum = 0;
     float freq = 1.0, amp = 1.0;
     float2 dsum = float2(0,0);
     for(int i=0; i < octaves; i++)
     {
         float3 n = perlinNoiseDeriv((p + warp * dsum)*freq, seed + i);
         sum += amp * (1 - abs(n.x));
         dsum += amp * n.yz * -n.x;
         freq *= lacunarity;
         amp *= gain * saturate(sum);
    }
    return sum;
}

float jordanTurbulence(float2 p, float seed, int octaves, float lacunarity = 2.0,
                       float gain1 = 0.8, float gain = 0.5,
                       float warp0 = 0.4, float warp = 0.35,
                       float damp0 = 1.0, float damp = 0.8,
                       float damp_scale = 1.0)
{
    float3 n = perlinNoiseDeriv(p, seed);
    float3 n2 = n * n.x;
    float sum = n2.x;
    float2 dsum_warp = warp0*n2.yz;
    float2 dsum_damp = damp0*n2.yz;

    float amp = gain1;
    float freq = lacunarity;
    float damped_amp = amp * gain;

    for(int i=1; i < octaves; i++)
    {
        n = perlinNoiseDeriv(p * freq + dsum_warp.xy, seed + i / 256.0);
        n2 = n * n.x;
        sum += damped_amp * n2.x;
        dsum_warp += warp * n2.yz;
        dsum_damp += damp * n2.yz;
        freq *= lacunarity;
        amp *= gain;
        damped_amp = amp * (1-damp_scale/(1+dot(dsum_damp,dsum_damp)));
    }
    return sum;
}
