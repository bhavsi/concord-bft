// Concord
//
// Copyright (c) 2018-2021 VMware, Inc. All Rights Reserved.
//
// This product is licensed to you under the Apache 2.0 license (the "License").
// You may not use this product except in compliance with the Apache 2.0 License.
//
// This product may include a number of subcomponents with separate copyright
// notices and license terms. Your use of these subcomponents is subject to the
// terms and conditions of the subcomponent's license, as noted in the LICENSE
// file.

#include "reconfiguration/reconfiguration_handler.hpp"
#include "reconfiguration/dispatcher.hpp"
#include "IRequestHandler.hpp"
#pragma once

namespace bftEngine {

class RequestHandler : public IRequestsHandler {
 public:
  RequestHandler() : reconfig_dispatcher_{std::make_shared<concord::reconfiguration::ReconfigurationHandler>()} {}

  void execute(ExecutionRequestsQueue &requests, const std::string &batchCid, concordUtils::SpanWrapper &) override;

  void setUserRequestHandler(std::shared_ptr<IRequestsHandler> userHdlr) {
    if (userHdlr) {
      userRequestsHandler_ = userHdlr;
      reconfig_handler_ = userHdlr->getReconfigurationHandler();
      pruning_handler_ = userHdlr->getPruningHandler();
      reconfig_dispatcher_.addReconfigurationHandler(userHdlr->getReconfigurationHandler());
      reconfig_dispatcher_.addPruningHandler(userHdlr->getPruningHandler());
    }
  }

  void setPruningHandler(std::shared_ptr<concord::reconfiguration::IPruningHandler> ph) override {
    reconfig_dispatcher_.addPruningHandler(ph);
  }

  void setReconfigurationHandler(std::shared_ptr<concord::reconfiguration::IReconfigurationHandler> rh) override {
    reconfig_dispatcher_.addReconfigurationHandler(rh);
  }
  void onFinishExecutingReadWriteRequests() override { userRequestsHandler_->onFinishExecutingReadWriteRequests(); }

 private:
  std::shared_ptr<IRequestsHandler> userRequestsHandler_ = nullptr;
  concord::reconfiguration::Dispatcher reconfig_dispatcher_;
};

}  // namespace bftEngine
