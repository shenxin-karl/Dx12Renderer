FORCEINLINE Frustum::Frustum(const float3 &origin, const float4 &orientation, float rightSlope,
	float leftSlope, float topSlope, float bottomSlope, float _near, float _far) noexcept
	: _boundingFrustum(origin, orientation,
		rightSlope, leftSlope,
		topSlope, bottomSlope,
		_near, _far)
{
}

FORCEINLINE Frustum::Frustum(const Matrix4 &projection) noexcept
	: _boundingFrustum(static_cast<DX::XMMATRIX>(projection))
{
}

FORCEINLINE Frustum Frustum::transform(const Matrix4 &trans) const noexcept {
	Frustum ret;
	_boundingFrustum.Transform(ret._boundingFrustum, static_cast<DX::XMMATRIX>(trans));
	return ret;
}

FORCEINLINE Frustum Frustum::transform(float scale, const Quaternion &rotate,
	const Vector3 &translation) const noexcept {
	Frustum ret;
	_boundingFrustum.Transform(ret._boundingFrustum, scale, rotate, translation);
	return ret;
}

FORCEINLINE std::array<float3, Frustum::kCornerCount> Frustum::getCorners() const noexcept {
	std::array<float3, Frustum::kCornerCount> ret;
	_boundingFrustum.GetCorners(ret.data());
	return ret;
}

FORCEINLINE void Frustum::getPlanes(Vector4 &nearPlane, Vector4 &farPlane,
	Vector4 &rightPlane, Vector4 &leftPlane,
	Vector4 topPlane, Vector4 &bottomPlane) const noexcept
{
	_boundingFrustum.GetPlanes(&nearPlane, &farPlane,
		&rightPlane, &leftPlane,
		&topPlane, &bottomPlane);
}

FORCEINLINE DX::ContainmentType Frustum::contains(const AxisAlignedBox &box) const noexcept {
	return _boundingFrustum.Contains(box._boundingBox);
}