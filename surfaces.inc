// Surfaces

/*
Sphere = Quadric
{
	square =	<+1, +1, +1>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	-1
}
*/

CylinderX = Quadric
{
	square =	< 0, +1, +1>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	-1
}

CylinderY = Quadric
{
	square =	<+1,  0, +1>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	-1
}

CylinderZ = Quadric
{
	square =	<+1, +1,  0>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	-1
}

ConeX = Quadric
{
	square =	<-1, +1, +1>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	0
}

ConeY = Quadric
{
	square =	<+1, -1, +1>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	0
}

ConeZ = Quadric
{
	square =	<+1, +1, -1>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0,  0>
	constant =	0
}

ParaboloidX = Quadric
{
	square =	< 0, +1, +1>
	mixed =		< 0,  0,  0>
	absolute =	<-1,  0,  0>
	constant =	0
}

ParaboloidY = Quadric
{
	square =	<+1,  0, +1>
	mixed =		< 0,  0,  0>
	absolute =	< 0, -1,  0>
	constant =	0
}

ParaboloidZ = Quadric
{
	square =	<+1, +1,  0>
	mixed =		< 0,  0,  0>
	absolute =	< 0,  0, -1>
	constant =	0
}
