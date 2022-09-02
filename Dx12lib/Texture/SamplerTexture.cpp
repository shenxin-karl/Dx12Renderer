#include <Dx12lib/Texture/SamplerTexture.h>
#include <Dx12lib/Resource/ResourceStateTracker.h>
#include <Dx12lib/Device/Device.h>

namespace dx12lib {

WRL::ComPtr<ID3D12Resource> SamplerTexture2D::getD3DResource() const {
	return _pResource;
}

const ShaderResourceView & SamplerTexture2D::getSRV(size_t mipSlice) const {
	if (_srvMgr.exist(mipSlice))
		return _srvMgr.get(mipSlice);

	assert(mipSlice < _pResource->GetDesc().MipLevels);
	auto pSharedDevice = _pDevice.lock();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _pResource.Get()->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	srvDesc.Texture2D.PlaneSlice = 0;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = static_cast<float>(mipSlice);

	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		_pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	_srvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

SamplerTexture2D::~SamplerTexture2D() {
	if (auto pSharedDevice = _pDevice.lock()) {
		if (auto *pGlobalResourceState = pSharedDevice->getGlobalResourceState())
			pGlobalResourceState->removeGlobalResourceState(_pResource.Get());
	}
}

SamplerTexture2D::SamplerTexture2D(std::weak_ptr<Device> pDevice, WRL::ComPtr<ID3D12Resource> pResource,
                                   WRL::ComPtr<ID3D12Resource> pUploader, D3D12_RESOURCE_STATES state)
: _pDevice(pDevice), _pResource(pResource), _pUploader(pUploader)
{
	assert(pResource->GetDesc().DepthOrArraySize == 1);
	auto pSharedDevice = pDevice.lock();
	pSharedDevice->getGlobalResourceState()->addGlobalResourceState(_pResource.Get(), state);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

WRL::ComPtr<ID3D12Resource> SamplerTexture2DArray::getD3DResource() const {
	return _pResource;
}

const ShaderResourceView & SamplerTexture2DArray::getSRV(size_t mipSlice) const {
	if (_srvMgr.exist(mipSlice))
		return _srvMgr.get(mipSlice);

	assert(mipSlice < _pResource->GetDesc().MipLevels);
	auto pSharedDevice = _pDevice.lock();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _pResource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = static_cast<float>(mipSlice);
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = _pResource->GetDesc().DepthOrArraySize;

	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		_pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	_srvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

const ShaderResourceView & SamplerTexture2DArray::getPlaneSRV(size_t planeSlice, size_t mipSlice) const {
	ViewManager<ShaderResourceView> &planeSrvMgr = _planeSrvMgr[planeSlice];
	if (planeSrvMgr.exist(mipSlice))
		return planeSrvMgr.get(mipSlice);

	assert(mipSlice < _pResource->GetDesc().MipLevels);
	auto pSharedDevice = _pDevice.lock();
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _pResource->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = static_cast<float>(mipSlice);
	srvDesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(planeSlice);
	srvDesc.Texture2DArray.ArraySize = 1;
	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		_pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);
	ShaderResourceView SRV(descriptor, this);
	planeSrvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

SamplerTexture2DArray::~SamplerTexture2DArray() {
	if (auto pSharedDevice = _pDevice.lock()) {
		if (auto *pGlobalResourceState = pSharedDevice->getGlobalResourceState())
			pGlobalResourceState->removeGlobalResourceState(_pResource.Get());
	}
}

SamplerTexture2DArray::SamplerTexture2DArray(std::weak_ptr<Device> pDevice, WRL::ComPtr<ID3D12Resource> pResource,
                                             WRL::ComPtr<ID3D12Resource> pUploader, D3D12_RESOURCE_STATES state)
: _pDevice(pDevice), _pResource(pResource), _pUploader(pUploader) {
	assert(pResource->GetDesc().DepthOrArraySize >= 1);
	auto pSharedDevice = pDevice.lock();
	pSharedDevice->getGlobalResourceState()->addGlobalResourceState(_pResource.Get(), state);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

WRL::ComPtr<ID3D12Resource> SamplerTextureCube::getD3DResource() const {
	return _pResource;
}

const ShaderResourceView & SamplerTextureCube::getSRV(size_t mipSlice) const {
	if (_srvMgr.exist(mipSlice))
		return _srvMgr.get(mipSlice);

	assert(mipSlice < _pResource->GetDesc().MipLevels);
	auto pSharedDevice = _pDevice.lock();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _pResource.Get()->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = -1;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.ResourceMinLODClamp = static_cast<float>(mipSlice);

	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		_pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	_srvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

const ShaderResourceView & SamplerTextureCube::getFaceSRV(CubeFace face, size_t mipSlice) const {
	ViewManager<ShaderResourceView> &faceSrvMgr = _faceMapSrvMgr[face];
	if (faceSrvMgr.exist(mipSlice))
		return faceSrvMgr.get(mipSlice);

	auto pSharedDevice = _pDevice.lock();
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _pResource.Get()->GetDesc().Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = -1;
	srvDesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(face);
	srvDesc.Texture2DArray.ArraySize = 1;
	srvDesc.Texture2DArray.PlaneSlice = 0;
	srvDesc.Texture2DArray.ResourceMinLODClamp = static_cast<float>(mipSlice);
	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	pSharedDevice->getD3DDevice()->CreateShaderResourceView(
		_pResource.Get(),
		&srvDesc,
		descriptor.getCPUHandle()
	);

	ShaderResourceView SRV(descriptor, this);
	faceSrvMgr.set(mipSlice, SRV);
	return _srvMgr.get(mipSlice);
}

SamplerTextureCube::~SamplerTextureCube() {
	if (auto pSharedDevice = _pDevice.lock()) {
		if (auto *pGlobalResourceState = pSharedDevice->getGlobalResourceState())
			pGlobalResourceState->removeGlobalResourceState(_pResource.Get());
	}
}

SamplerTextureCube::SamplerTextureCube(std::weak_ptr<Device> pDevice, WRL::ComPtr<ID3D12Resource> pResource,
                                       WRL::ComPtr<ID3D12Resource> pUploader, D3D12_RESOURCE_STATES state)
: _pDevice(pDevice), _pResource(pResource), _pUploader(pUploader)
{
	assert(pResource->GetDesc().DepthOrArraySize == 6);
	auto pSharedDevice = pDevice.lock();
	pSharedDevice->getGlobalResourceState()->addGlobalResourceState(_pResource.Get(), state);
}

}
