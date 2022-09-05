FORCEINLINE AxisAlignedBox::AxisAlignedBox(const Vector3 &min, const Vector3 &max) noexcept {
	DX::BoundingBox::CreateFromPoints(_boundingBox, min, max);
}

FORCEINLINE AxisAlignedBox::AxisAlignedBox(const DX::BoundingBox &box) noexcept : _boundingBox(box) {
}

FORCEINLINE AxisAlignedBox & AxisAlignedBox::operator=(const DX::BoundingBox &box) noexcept {
	_boundingBox = box;
	return *this;
}

FORCEINLINE AxisAlignedBox AxisAlignedBox::transform(const Matrix4 &trans) const noexcept {
	AxisAlignedBox ret;
	_boundingBox.Transform(ret._boundingBox, static_cast<DX::XMMATRIX>(trans));
	return ret;
}

FORCEINLINE AxisAlignedBox AxisAlignedBox::transform(float scale, const Quaternion &rotate, const Vector3 &translation) const noexcept {
	AxisAlignedBox ret;
	_boundingBox.Transform(ret._boundingBox, scale, rotate, translation);
	return ret;
}

FORCEINLINE std::array<float3, AxisAlignedBox::kCornerCount> AxisAlignedBox::getCorners() const noexcept {
	std::array<float3, AxisAlignedBox::kCornerCount> ret;
	_boundingBox.GetCorners(ret.data());
	return ret;
}

FORCEINLINE void AxisAlignedBox::getMinMax(Vector3 &min, Vector3 &max) const noexcept {
	Vector3 center = Vector3(_boundingBox.Center.x, _boundingBox.Center.y, _boundingBox.Center.z);
	Vector3 extent = Vector3(_boundingBox.Extents.x, _boundingBox.Extents.y, _boundingBox.Extents.z);
	min = center - extent;
	max = center + extent;
}

FORCEINLINE AxisAlignedBox AxisAlignedBox::createFromCenter(const float3 &center, const float3 &extents) noexcept {
	AxisAlignedBox ret;
	ret._boundingBox = DX::BoundingBox(center, extents);
	return ret;
}

FORCEINLINE AxisAlignedBox AxisAlignedBox::createFromPoints(size_t count, const float3 *points, size_t stride) noexcept {
	AxisAlignedBox ret;
	DX::BoundingBox::CreateFromPoints(ret._boundingBox, count, points, stride);
	return ret;
}

FORCEINLINE AxisAlignedBox AxisAlignedBox::createMerged(const AxisAlignedBox &b1, const AxisAlignedBox &b2) noexcept {
	AxisAlignedBox ret;
	DX::BoundingBox::CreateMerged(ret._boundingBox, b1._boundingBox, b2._boundingBox);
	return ret;
}