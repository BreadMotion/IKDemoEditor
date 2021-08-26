
#define sizeX 64
#define sizeY 1
#define sizeZ 1

#define COMPARISON(a, b) ( a.key < b.key )

struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

cbuffer ConstantBuffer : register(b0)
{
    float inc;
    float dir;
    float2 dummy;
};

struct SortData
{
    float key;
    uint index;
};
RWStructuredBuffer<SortData> data : register(u0);

SortData LerpOfSortData(SortData d0, SortData d1, float s)
{
    SortData dr;
    
    dr.key = lerp(d0.key, d1.key, s);
    dr.index = lerp(d0.index, d1.index, s);
    
    return dr;
}

[numthreads(sizeX, sizeY, sizeZ)]
void main(uint threadid : SV_DispatchThreadID)
{
    int incI = (int)inc;
    int dirI = (int)dir;
    
    int t = threadid; // thread index
    int low = t & (incI - 1); // low order bits (below INC)
    int i = (t << 1) - low; // insert 0 at position INC
    bool reverse = ((dirI & i) == 0); // asc/desc order

	// Load
    SortData x0 = data[i];
    SortData x1 = data[incI + i];

	// Sort
    bool swap = reverse ^ COMPARISON(x0, x1);
    
    SortData auxa = x0;
    SortData auxb = x1;
    
    x0 = LerpOfSortData(x0, auxb, (float)swap);
    x1 = LerpOfSortData(x1, auxa, (float)swap);
    
	// Store
    data[i] = x0;
    data[incI + i] = x1;
}