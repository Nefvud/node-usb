#ifndef SRC_ENDPOINT_H
#define SRC_ENDPOINT_H

#include "bindings.h"

namespace NodeUsb {
	class Endpoint;

	class Callback {
		public:
			// Dispatcher / callback handler must be static
			static void DispatchAsynchronousUsbTransfer(libusb_transfer *_transfer);
	};

	struct endpoint_request:nodeusb_transfer {
		Endpoint * endpoint;
	};

	struct bulk_interrupt_transfer_request:endpoint_request {
		int length;
		int transferred;
	};

	class Endpoint : public ObjectWrap {
		public:
			static void Initalize(Handle<Object> target);
			static Persistent<FunctionTemplate> constructor_template;
			Endpoint(Handle<Object>, nodeusb_device_container*, const libusb_endpoint_descriptor*, uint32_t);
			~Endpoint();

		protected:
			// members
			Persistent<Object> device;
			struct nodeusb_device_container *device_container;
			const struct libusb_endpoint_descriptor *descriptor;

			int32_t endpoint_type;
			uint32_t transfer_type;
			uint32_t idx_endpoint;

			int FillTransferStructure(libusb_transfer *_transfer, unsigned char *_buffer, int32_t _buflen, Persistent<Function> _callback, uint32_t _timeout, unsigned int num_iso_packets = 0);

			// v8 getter
			static Handle<Value> EndpointTypeGetter(Local<String> property, const AccessorInfo &info);
			static Handle<Value> TransferTypeGetter(Local<String> property, const AccessorInfo &info);
			static Handle<Value> MaxPacketSizeGetter(Local<String> property, const AccessorInfo &info);
			static Handle<Value> MaxIsoPacketSizeGetter(Local<String> property, const AccessorInfo &info);
			// exposed to V8
			static Handle<Value> New(const Arguments& args);
			static Handle<Value> Submit(const Arguments& args);
			static Handle<Value> GetExtraData(const Arguments& args);
			static Handle<Value> BulkTransfer(const Arguments& args);
			static void EIO_BulkTransfer(uv_work_t *req);
			static void EIO_After_BulkTransfer(uv_work_t *req);
			static Handle<Value> InterruptTransfer(const Arguments& args);
			static void EIO_InterruptTransfer(uv_work_t *req);
			static void EIO_After_InterruptTransfer(uv_work_t *req);

	};
	
}

#endif
