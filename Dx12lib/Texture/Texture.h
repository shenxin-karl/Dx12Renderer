#pragma once
#include <variant>
#include <Dx12lib/dx12libStd.h>
#include "Dx12lib/Resource/IResource.h"

namespace dx12lib {

enum class TextureDimension {
	UNKNOWN = 0,
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D,
};

class Texture : public IResource {
public:
	Texture(std::weak_ptr<Device> pDevice, const D3D12_RESOURCE_DESC &desc, const D3D12_CLEAR_VALUE *pClearValue = nullptr);
	Texture(std::weak_ptr<Device> pDevice, WRL::ComPtr<ID3D12Resource> pResource, const D3D12_CLEAR_VALUE *pClearValue = nullptr);
	auto getDimension() const -> TextureDimension;
	auto getDesc() const -> const D3D12_RESOURCE_DESC &;
	auto getArraySize() const -> size_t;
	auto getSRV(size_t mipSlice = 0) const -> const ShaderResourceView *;
	auto getUAV(size_t mipSlice = 0) const -> const UnorderedAccessView *;
	auto getRTV(size_t mipSlice = 0) const -> const RenderTargetView *;
	auto getDSV(size_t mipSlice = 0) const -> const DepthStencilView *;
	auto getPlaneSRV(size_t planeSlice, size_t mipSlice = 0) const -> const ShaderResourceView *;
	auto getPlaneUAV(size_t planeSlice, size_t mipSlice = 0) const -> const UnorderedAccessView *;
	auto getPlaneRTV(size_t planeSlice, size_t mipSlice = 0) const -> const RenderTargetView *;
	auto getPlaneDSV(size_t planeSlice, size_t mipSlice = 0) const -> const DepthStencilView *;
	auto getCubeSRV(size_t mipSlice = 0) const -> const ShaderResourceView *;
	auto getCubeUAV(size_t mipSlice = 0) const -> const UnorderedAccessView *;
	auto getCubeRTV(size_t mipSlice = 0) const -> const RenderTargetView *;
	auto getCubeDSV(size_t mipSlice = 0) const -> const DepthStencilView *;
	~Texture() override;
	static D3D12_RESOURCE_DESC make2D(size_t width, size_t height, 
		D3D12_RESOURCE_FLAGS flags, size_t numMipMap = 1
	);
	static D3D12_RESOURCE_DESC make2DArray(size_t width, size_t height, size_t arraySize, 
		D3D12_RESOURCE_FLAGS flags, size_t numMipMap = 1
	);
	static D3D12_RESOURCE_DESC makeCube(size_t width, size_t height,
		D3D12_RESOURCE_DESC flags, size_t numMipMap = 1
	);
	bool checkSRVSupport() const;
	bool checkUAVSupport() const;
	bool checkRTVSupport() const;
	bool checkDSVSupport() const;
private:
	struct ViewKey;
	enum class ViewType;
	using ViewMap = std::unordered_map<ViewKey,
		std::variant<ShaderResourceView, UnorderedAccessView, RenderTargetView, DepthStencilView>
	>;
	void initFeatureSupport(ID3D12Device *pDevice, DXGI_FORMAT format);
	bool checkFormatSupport(D3D12_FORMAT_SUPPORT1 formatSupport1) const;
	bool checkFormatSupport(D3D12_FORMAT_SUPPORT2 formatSupport2) const;
private:
	ViewMap _viewMap;
	D3D12_CLEAR_VALUE _clearValue;
	D3D12_RESOURCE_DESC _resourceDesc;
	WRL::ComPtr<ID3D12Resource> _pResource;
	D3D12_FEATURE_DATA_FORMAT_SUPPORT _formatSupport;
};

enum class Texture::ViewType {
	Unknown = 0,
	SRV     = 1,
	UAV     = 2,
	RTV     = 3,
	DSV     = 4,
};

struct Texture::ViewKey {
	ViewType viewType;
	ShaderResourceDimension dimension;
	size_t mipSlice;
	size_t planeSlice;
	size_t first;

	struct Hasher {
		using result_type = std::size_t;
		result_type operator()(const ViewKey &key) const {
			std::hash<std::string_view> hasher;
			auto strView = std::string_view(reinterpret_cast<const char *>(&key), sizeof(key));
			return hasher(strView);
		}
	};
public:
	ViewKey();
	friend bool operator==(const ViewKey &, const ViewKey &);
};

}


