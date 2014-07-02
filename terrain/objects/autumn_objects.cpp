#include "trees.hpp"

#include <library/math/vector.hpp>
#include "blocks.hpp"
#include "generator.h"
#include "helpers.hpp"
#include "random.hpp"
#include <cmath>

using namespace library;

void ingenBranch(int gx, int gy, int gz, int height, float lowrad)
{
	float min_rad = 0.25;
	if (lowrad < min_rad) lowrad = min_rad;
	if (height < 1) return;
	
	float toprad = min_rad;
	
	const float extraleaf = 3.0;
	int startrad = height / 7.0;
	
	const float jitter = 1.5f;
	float min_jitter   = 0.1f;
	float interpolback = 0.9f;
	
	float jitter_x = randf(gx+1, gy-221, gz) * jitter - jitter * 0.5;
	float jitter_z = randf(gx-1, gy-120, gz) * jitter - jitter * 0.5;
	float x = 0.0f;
	float z = 0.0f;
	
	for (float y = 0.0; y <= height + extraleaf; y += 0.5f)
	{
		float currad = (height - y) / (float)height;
		currad = mix(toprad, lowrad, currad * currad);
		
		x += jitter_x; jitter_x *= interpolback;
		z += jitter_z; jitter_z *= interpolback;
		
		// leaves
		if (y >= startrad)
		{
			float length  = (height + extraleaf) - startrad;
			float leafrad = (y - startrad) / length;
			leafrad = std::sin( leafrad * PI);
			
			float blobsize = 0.5 + length * 0.25;
			
			ocircleXZstencil(gx+x, gy+y, gz+z, 
				leafrad * blobsize, _LEAF_LEAFS, 0.9);
		}
		
		// re-jitter
		if (std::abs(jitter_x) < min_jitter)
			jitter_x = randf(gx+x, gy+y-95, gz-z) * jitter - jitter * 0.5;
		if (std::abs(jitter_z) < min_jitter)
			jitter_z = randf(gx+x, gy+y-75, gz+z) * jitter - jitter * 0.5;
		
		if (y <= height)
		{
			// trunk
			ocircleXZ(gx+x, gy+y, gz+z, currad, _WOODBROWN);
		}
	}
}

void ingenBigDarkTree(int gx, int gy, int gz, int height)
{
	if (coretest(gx, gy, gz, 0, 2, height) == false) return;
	
	float lowrad = height / 15.0;
	ingenBranch(gx, gy, gz, height, lowrad);
}

/*
Sub ingenCactusPart(s As sector Ptr, bx As Short, by As Short, bz As Short, height As Integer, rad As Single, jitfact As glfloat = 1.0, coretest As Integer = FALSE)
	
	Dim As Single currad = rad
	Dim As Single x, z
	If height < 1 Then Return
	
	Const As Single jitter = 2.5
	Const As Single min_jitter = 0.1
	Const As Single interpolback = 0.075
	
	Dim As Single jitter_x = iRnd(s, bx+1, by, bz-1) * jitter - jitter * 0.5
	Dim As Single jitter_z = iRnd(s, bx-1, by, bz+1) * jitter - jitter * 0.5
	jitter_x *= jitfact
	jitter_z *= jitfact
	
	Dim As Single sphereparts = height * 0.8
	Const As Single minrad = 0.25
	Const As Single offshootrad = 0.6
	Const As Single min_offshootrad = 0.5
	
	Const As Integer offshootchance = 2 '1 in 2
	Const As Integer offshootlevel = 4
	
	For y As Integer = 0 To height
		
		If y > sphereparts Then
			currad = (y - sphereparts) / (height - sphereparts)
			currad = mix(rad, minrad, currad * currad)
		Else
			currad = rad
		EndIf
		
		x += jitter_x
		z += jitter_z
		jitter_x = mix(jitter_x, 0.0, interpolback)
		jitter_z = mix(jitter_z, 0.0, interpolback)
		
		're-jitter
		If Abs(jitter_x) < min_jitter Then
			jitter_x = iRnd(s, bx+x, by+y, bz-z) * jitter - jitter * 0.5
			jitter_x *= jitfact
		EndIf
		If Abs(jitter_z) < min_jitter Then
			jitter_z = iRnd(s, bx-x, by+y, bz+z) * jitter - jitter * 0.5
			jitter_z *= jitfact
		EndIf
		
		If Abs(y - height\2) < offshootlevel And currad > min_offshootrad Then
			
			If Int(iRnd(s, bx+x, by+y, bz+z) * offshootchance) = 0 Then
				ingenCactusPart(s, bx + x, by + y, bz + z, iRnd(s, bx+x, by+y, bz+z) * (height-y), currad * offshootrad)
			EndIf
			
		EndIf
		
		If y <= height Then		
			'trunk
			ingenCircle(s, bx + x, by + y, bz + z, _CACTUS, currad, FALSE, y = 0 And coretest)
			
			If y = height Then
				
				setblock2(s, bx+x, by+y+1, bz+z, _DESERTFLOWER)
				
			EndIf
			
		EndIf
		
	Next
	
End Sub

Sub ingenCactus(ByRef w As work)
	
	Dim As Integer height = Int(iRnd(w.s, w.bx, w.by, w.bz) * 40) + 25
	Dim As Integer cacrad = (height / 25) * 2
	
	ingenCactusPart(w.s, w.bx, w.by, w.bz, height, cacrad, 0.5f, TRUE)
	
End Sub
*/