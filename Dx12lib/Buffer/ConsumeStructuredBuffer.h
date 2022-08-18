#pragma once
#include <Dx12lib/Device/Device.h>
#include <Dx12lib/Buffer/UploadBuffer.h>
#include <Dx12lib/Resource/ResourceView.hpp>
#include <Dx12lib/Resource/IResource.h>
#include <span>

namespace dx12lib {

class ConsumeStructuredBuffer : public IConsumeStructuredBuffer {
public:
	WRL::ComPtr<ID3D12Resource> getD3DResource() const override;
	size_t getBufferSize() const override;
	size_t getElementCount() const override;
	size_t getElementStride() const override;
	UnorderedAccessView getUAV() const override;
protected:
	ConsumeStructuredBuffer(std::weak_ptr<Device> pDevice, std::shared_ptr<CommandList> pCmdList, const void *pData, size_t numElements, size_t stride);
private:
	size_t _elementStride;
	UnorderedAccessView _uav;
	std::unique_ptr<DefaultBuffer> _pDefaultBuffer;
};

}
