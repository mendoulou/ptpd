/*-
 * libCCK - Clock Construction Kit
 *
 * Copyright (c) 2014 Wojciech Owczarek,
 *
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file 	   cck_acl.c
 * 
 * @brief  	   constructor and destructor for ACL implementations
 * 
 * 
 */

#include "cck.h"

/* Quick shortcut to call the setup function for different implementations */
#define CCK_REGISTER_ACL(type,suffix) \
	if(aclType==type) {\
	    cckAclSetup_##suffix(acl);\
	}

CckLogHandler* cckLogOutput;

CckAcl*
createCckAcl(int aclType, int processingOrder, const char* instanceName)
{

    CckAcl* acl;

    CCKCALLOC(acl, sizeof(CckAcl));

    acl->header._next = NULL;
    acl->header._prev = NULL;
    acl->header._dynamic = CCK_TRUE;

    setupCckAcl(acl, aclType, processingOrder, instanceName);

    return acl;
}

void
setupCckAcl(CckAcl* acl, int aclType, int processingOrder, const char* instanceName)
{

    acl->header.componentType = CCK_COMPONENT_ACL;

    strncpy(acl->header.instanceName, instanceName, CCK_MAX_INSTANCE_NAME_LEN);

    acl->aclType = aclType;
    acl->processingOrder = processingOrder;

/*
   if you write an ACL handler for a new transport type,
   make sure you register it here. Format is:

   CCK_REGISTER_ACL(TRANSPORT_TYPE, function_name_suffix)

   where suffix is the string the setup() function name is
   suffixed with for your implementation, i.e. you
   need to define cckAclSetup_bob for your "bob" tansport ACL
*/

    CCK_REGISTER_ACL(CCK_ACL_NULL, null);
    CCK_REGISTER_ACL(CCK_ACL_IPV4, ipv4);
    CCK_REGISTER_ACL(CCK_ACL_IPV6, ipv6);
    CCK_REGISTER_ACL(CCK_ACL_ETHERNET, ethernet);

/* we 're done with this macro */
#undef CCK_REGISTER_ACL

    acl->header._next = NULL;
    acl->header._prev = NULL;
    cckRegister(acl);

}

void freeCckAcl(CckAcl** acl)
{
	if(*acl==NULL)
	    return;

	(*acl)->shutdown(*acl);

	cckDeregister(*acl);

	if((*acl)->header._dynamic) {
	    free(*acl);
	    *acl = NULL;
	}
}

