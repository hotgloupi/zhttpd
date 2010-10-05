
#ifndef __CONSTANTS_HPP__
# define __CONSTANTS_HPP__

namespace ZHTTPD
{
    namespace API
    {
        namespace EVENT
        {
            /**
             * @enum ZHTTPD::API::EVENT::Type
             * Évènement reçu en premier argument par la
             * méthode IModule::processRequest.
             */
            enum Type
            {
                /**
                 * Informe le module qu'il peut lire sur la socket
                 * sans bloquer. Seul le module d'entrée est susceptible
                 * de recevoir cet évènement.
                 */
                ON_CAN_READ         = 1,

                /**
                 * Informe le module qu'il peut écrire sur la socket
                 * sans bloquer. Seul le module de sortie est susceptible
                 * de recevoir cet évènement.
                 */
                ON_CAN_WRITE        = 2,

                /**
                 * Le module a de nouvelles données à traiter. Le module
                 * d'entrée ne peut pas recevoir cet évènement.
                 */
                ON_REQUEST_DATA     = 3,
                ON_RESPONSE_DATA    = 4,

                /**
                 * Le module avait demandé à être rappelé plus tard
                 * (avec IRequest::callLater()).
                 */
                ON_IDLE             = 5,

                /**
                 * La requête est en train de se terminer, c'est le dernier évènement
                 * reçu par le module.
                 */
                ON_END              = 6,

                /**
                 * La requête est en erreur
                 */
                ON_ERROR            = 7,
            };
        }

        namespace HTTP_METHOD
        {
#undef DELETE
            /**
             * Toutes les méthodes HTTP.
             */
            enum Type
            {
                UNDEFINED   = 0,
                GET         = 1,
                POST        = 2,
                PUT         = 3,
                DELETE      = 4,
                TRACE       = 5,
                OPTIONS     = 6,
                CONNECT     = 7,
                HEAD        = 8,
            };
        }

        namespace HTTP_CODE
        {
            /**
             * Tous les codes de retour standards.
             */
            enum Type
            {
                UNDEFINED                       = 0,
                CONTINUE                        = 100,
                SWITCHING_PROTOCOLS             = 101,
                PROCESSING                      = 102,
                OK                              = 200,
                CREATED                         = 201,
                ACCEPTED                        = 202,
                NON_AUTHORITATIVE_INFORMATION   = 203,
                NO_CONTENT                      = 204,
                RESET_CONTENT                   = 205,
                PARTIAL_CONTENT                 = 206,
                MULTI_STATUS                    = 207,
                CONTENT_DIFFERENT               = 210,
                MULTIPLE_CHOICES                = 300,
                MOVED_PERMANENTLY               = 301,
                FOUND                           = 302,
                SEE_OTHER                       = 303,
                NOT_MODIFIED                    = 304,
                USE_PROXY                       = 305,
                TEMPORARY_REDIRECT              = 307,
                TOO_MANY_REDIRECT               = 310,
                BAD_REQUEST                     = 400,
                UNAUTHORIZED                    = 401,
                PAYMENT_REQUIRED                = 402,
                FORBIDDEN                       = 403,
                NOT_FOUND                       = 404,
                METHOD_NOT_ALLOWED              = 405,
                NOT_ACCEPTABLE                  = 406,
                PROXY_AUTHENTICATION_REQUIRED   = 407,
                REQUEST_TIME_OUT                = 408,
                CONFLICT                        = 409,
                GONE                            = 410,
                LENGTH_REQUIRED                 = 411,
                PRECONDITION_FAILED             = 412,
                REQUEST_ENTITY_TOO_LARGE        = 413,
                REQUEST_URI_TOO_LONG            = 414,
                UNSUPPORTED_MEDIA_TYPE          = 415,
                REQUESTED_RANGE_UNSATISFIABLE   = 416,
                EXPECTATION_FAILED              = 417,
                UNPROCESSABLE_ENTITY            = 422,
                LOCKED                          = 423,
                METHOD_FAILURE                  = 424,
                UNORDERED_COLLECTION            = 425,
                UPGRADE_REQUIRED                = 426,
                INTERNAL_SERVER_ERROR           = 500,
                NOT_IMPLEMENTED                 = 501,
                BAD_GATEWAY                     = 502,
                SERVICE_UNAVAILABLE             = 503,
                GATEWAY_TIME_OUT                = 504,
                HTTP_VERSION_NOT_SUPPORTED      = 505,
                INSUFFICIENT_STORAGE            = 507,
                BANDWIDTH_LIMIT_EXCEEDED        = 509,
            };
        }

        namespace CATEGORY
        {
            /**
             * Catégorie à laquelle appartient un module.
             */
            enum Type
            {
                UNDEFINED               = 0,
                INPUTOUTPUT             = 1,
                POSTINPUT               = 2,
                PREBUILD                = 3,

                PREREQUESTFILTER        = 4,
                UNCOMPRESS              = 5,
                POSTREQUESTFILTER       = 6,

                PREPROCESSING           = 7,
                PROCESSING              = 8,
                POSTPROCESSING          = 9,


                PRERESPONSEFILTER       = 10,
                COMPRESS                = 11,
                POSTRESPONSEFILTER      = 12,

                PREOUTPUT               = 13,
            };
        }
    }
}

#endif /* !__CONSTANTS_HPP__ */

