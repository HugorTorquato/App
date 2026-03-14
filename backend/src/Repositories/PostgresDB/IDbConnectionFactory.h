#pragma once

#include <memory>

#include "IDbSession.h"

class IDbConnectionFactory {
   public:
    virtual ~IDbConnectionFactory() = default;

    virtual std::unique_ptr<IDbSession> createSession() = 0;
};