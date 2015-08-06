struct Pixel
{
	int color;
};

StructuredBuffer<Pixel> Buffer0 : register(t0);
RWStructuredBuffer<Pixel> BufferOut : register(u0);

float3 readPixel(int x, int y)
{
	float3 output;
	uint index = x + y * 1024;

	output.x = (float)(((Buffer0[index].color)&0x000000ff)      ) / 255.0f;
	output.y = (float)(((Buffer0[index].color)&0x0000ff00) >> 8 ) / 255.0f;
	output.z = (float)(((Buffer0[index].color)&0x00ff0000) >> 16) / 255.0f;

	return output;
}

void writeToPixel(int x, int y, float3 color)
{
	uint index = x + y * 1024;

	int ired   = (int)(clamp(color.r, 0, 1) * 255);
	int igreen = (int)(clamp(color.g, 0, 1) * 255) << 8;
	int iblue  = (int)(clamp(color.b, 0, 1) * 255) << 16;

	BufferOut[index].color = ired + igreen + iblue;
}

[numthreads(32, 16, 1)]
void main( uint3 dispatchThreadID : SV_DispatchThreadID )
{
	float3 pixel = readPixel(dispatchThreadID.x, dispatchThreadID.y);
	pixel.rgb = pixel.r * 0.3 + pixel.g * 0.59 + pixel.b * 0.11;
	writeToPixel(dispatchThreadID.x, dispatchThreadID.y, pixel);
}
