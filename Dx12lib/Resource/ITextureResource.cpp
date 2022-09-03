#include "ITextureResource.h"
#include "ResourceStateTracker.h"
#include "Dx12lib/Device/Device.h"

namespace dx12lib {

/// *********************************** ITextureResource ****************************************
#if 1

size_t ITextureResource::getMipLevels() const {
	return getD3DResource()->GetDesc().MipLevels;
}

ShaderResourceDimension ITextureResource2D::getDimension() const {
	return ShaderResourceDimension::Texture2D;
}

const ShaderResourceView & ITextureResource2D::getSRV(size_t mipSlice) const {
	auto pResource = getD3DResource();
	assert(mipSlice < pResource->GetDesc().MipLevels);
	if (_srvMgr.exist(mipSlice))
		return _srvMgr.get(mipSlice);

	auto pSharedDevice = getDevice().lock();
	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = pResource->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = static_cast<float>(mipSlice);

	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	_srvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

ShaderResourceDimension ITextureResource2DArray::getDimension() const {
	return ShaderResourceDimension::Texture2DArray;
}

size_t ITextureResource2DArray::getPlaneSlice() const {
	return getD3DResource()->GetDesc().DepthOrArraySize;
}
#endif

/// *********************************** ITextureResource2DArray ****************************************
#if 1
const ShaderResourceView & ITextureResource2DArray::getSRV(size_t mipSlice) const {
	if (_srvMgr.exist(mipSlice))
		return _srvMgr.get(mipSlice);

	auto pResource = getD3DResource();
	auto resourceDesc = pResource->GetDesc();
	assert(mipSlice < resourceDesc.MipLevels);
	auto pSharedDevice = getDevice().lock();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = resourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = static_cast<float>(mipSlice);
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = resourceDesc.DepthOrArraySize;

	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	_srvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

const ShaderResourceView & ITextureResource2DArray::getPlaneSRV(size_t planeSlice, size_t mipSlice) const {
	auto pResource = getD3DResource();
	assert(planeSlice < getPlaneSlice());
	ViewManager<ShaderResourceView> &planeSrvMgr = _planeSrvMgr[planeSlice];
	if (planeSrvMgr.exist(mipSlice))
		return planeSrvMgr.get(mipSlice);

	auto pSharedDevice = getDevice().lock();
	auto resourceDesc = pResource->GetDesc();
	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = resourceDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(planeSlice);
	srvDesc.Texture2DArray.ArraySize = 1;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = static_cast<float>(mipSlice);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	planeSrvMgr.set(mipSlice, SRV);
	return planeSrvMgr.get(mipSlice);
}

ShaderResourceDimension ITextureResourceCube::getDimension() const {
	return ShaderResourceDimension::TextureCube;
}
#endif

/// *********************************** ITextureResourceCube ****************************************

#if 1
const ShaderResourceView & ITextureResourceCube::getSRV(size_t mipSlice) const {
	if (_srvMgr.exist(mipSlice))
		return _srvMgr.get(mipSlice);

	auto pResource = getD3DResource();
	auto resourceDesc = pResource->GetDesc();
	assert(mipSlice < resourceDesc.MipLevels);
	auto pSharedDevice = getDevice().lock();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = resourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = -1;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.ResourceMinLODClamp = static_cast<float>(mipSlice);

	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	_srvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

const ShaderResourceView & ITextureResourceCube::getFaceSRV(CubeFace face, size_t mipSlice) const {
	ViewManager<ShaderResourceView> &cubeSrvMgr = _cubeSrvMgr[face];
	if (cubeSrvMgr.exist(mipSlice))
		return cubeSrvMgr.get(mipSlice);

	auto pResource = getD3DResource();
	auto pSharedDevice = getDevice().lock();
	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = pResource->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(face);
	srvDesc.Texture2DArray.ArraySize = 1;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = static_cast<FLOAT>(mipSlice);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	cubeSrvMgr.set(mipSlice, SRV);
	return cubeSrvMgr.get(mipSlice);
}
#endif

}

