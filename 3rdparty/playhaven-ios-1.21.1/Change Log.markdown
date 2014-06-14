# Change Log
============

1.21.1
======
* Fixed issue with application freeze on iOS 7 that happened for push notifications linked with URI.

1.21.0
======
* Upgraded request signatures to v4-style.
* Removed ODIN1 and GID identifiers.
* Fixed new warnings reported by Xcode 5.
* IDFV parameter is now sent on all requests.
* Updated SDK requests to sent user opt-out flag to the server, which is used to fulfil COPPA and TRUSTe compliances.
* Added support for URLs with custom schemas triggered from announcements.

1.20.0
======
* Added support to Push Notifications which can be sent by means of PlayHaven Push Dashboard.

1.13.2
======
* Addresses an issue which causes content units to not work on iOS 4.3
* Addresses an issue with the error handling of PHAPIRequest.m

1.13.1
======
* MAC Address, ODIN1, customUDID parameters are now sent on all requests
* The customUDID parameter now strips RFC 3986 Reserved Characters
* More unit tests added and a warning fixed

1.13.0
======
* UDID collection has been removed to comply with Apple’s policy for the use of device information, beginning May 1, 2013
* Receipt verification available on the SDK with server-side component available soon.
* Miscellaneous bug fixes

1.12.1
======
* iOS 6 compatibility improvements
* In-App iTunes purchases support for content units.
* Fixes for crashes affecting devices running iOS versions lower than 5.0

1.12.0
======
* The SDK now automatically records the number of game sessions and the length of game sessions. This depends on a proper open request implementation.

1.11.0
======
* App Store launches now properly preserve affiliate link tokens
* Build settings changed to remove THUMB instructions from static library builds. This change only affects publishers using this SDK as a static library from the Unity plugin

1.10.4
======
* In-App Purchase (IAP) tracking requests now report accurate price information

1.10.3
======
* DNS resolution for API servers happens in a background thread

1.10.2
======
* Bugfixes for issues with canceling requests and a rare crash involving precaching

1.10.1
======
* Ability to opt out of user data collection at runtime

1.10.0
======
* In-App Purchase tracking and virtual goods promotion support.
* New documentation on how to disable Store Kit-based features in the SDK
