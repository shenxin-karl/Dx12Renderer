FORCEINLINE BoundingBox::BoundingBox(const Vector3 &min, const Vector3 &max) noexcept {
	DX::BoundingBox::CreateFromPoints(_boundingBox, min, max);
}

FORCEINLINE BoundingBox::BoundingBox(const DX::BoundingBox &other) noexcept : _boundingBox(other) {
}

FORCEINLINE BoundingBox::BoundingBox(const BoundingBox &lhs, const BoundingBox &rhs) noexcept {
	*this = createMerged(lhs, rhs);
}

FORCEINLINE BoundingBox::BoundingBox(const BoundingSphere &boundingSphere) noexcept {
	DX::BoundingBox::CreateFromSphere(baseCast(), boundingSphere.baseCast());
}

FORCEINLINE BoundingBox &BoundingBox::operator=(const DX::BoundingBox &other) noexcept {
	_boundingBox = other;
	return *this;
}

FORCEINLINE BoundingBox BoundingBox::transform(const Matrix4 &trans) const noexcept {
	BoundingBox ret;
	baseCast().Transform(ret.baseCast(), static_cast<DX::XMMATRIX>(trans));
	return ret;
}

FORCEINLINE BoundingBox BoundingBox::transform(float scale,
	const Quaternion &rotate,
	const Vector3 &translation) const noexcept
{
	BoundingBox ret;
	baseCast().Transform(ret.baseCast(), scale, rotate, translation);
	return ret;
}

FORCEINLINE std::array<float3, BoundingBox::kCornerCount> BoundingBox::getCorners() const noexcept {
	std::array<float3, BoundingBox::kCornerCount> corners;
	baseCast().GetCorners(corners.data());
	return corners;
}

FORCEINLINE void BoundingBox::getMinMax(Vector3 &min, Vector3 &max) const noexcept {
	Vector3 center(baseCast().Center);
	Vector3 extents(baseCast().Extents);
	min = center - extents;
	max = center + extents;
}

FORCEINLINE std::pair<Vector3, Vector3> BoundingBox::getMinMax() const noexcept {
	Vector3 vMin, vMax;
	getMinMax(vMin, vMax);
	return { vMin, vMax };
}

FORCEINLINE DX::BoundingBox &BoundingBox::baseCast() noexcept {
	return _boundingBox;
}

FORCEINLINE const DX::BoundingBox &BoundingBox::baseCast() const noexcept {
	return _boundingBox;
}

FORCEINLINE BoundingBox BoundingBox::createFromCenter(const float3 &center, const float3 &extents) noexcept {
	DX::BoundingBox box(center, extents);
	return { box };
}

FORCEINLINE BoundingBox BoundingBox::createFromPoints(size_t count, const float3 *points, size_t stride) noexcept {
	DX::BoundingBox box;
	DX::BoundingBox::CreateFromPoints(box, count, points, stride);
	return box;
}

FORCEINLINE BoundingBox BoundingBox::createMerged(const BoundingBox &b1, const BoundingBox &b2) noexcept {
	DX::BoundingBox box;
	DX::BoundingBox::CreateMerged(box, b1.baseCast(), b2.baseCast());
	return box;
}