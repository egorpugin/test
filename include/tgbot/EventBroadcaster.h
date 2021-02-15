#pragma once

#include "tgbot/Types.h"

#include <functional>
#include <initializer_list>
#include <unordered_map>

namespace tgbot
{

class EventHandler;

/**
 * @brief This class holds all event listeners.
 *
 * @ingroup general
 */
class TGBOT_API EventBroadcaster
{
public:
    typedef std::function<void(const Message&)> MessageListener;
    typedef std::function<void(const InlineQuery&)> InlineQueryListener;
    typedef std::function<void(const ChosenInlineResult&)> ChosenInlineResultListener;
    typedef std::function<void(const CallbackQuery&)> CallbackQueryListener;

    /**
     * @brief Registers listener which receives all messages which the bot can ever receive.
     * @param listener Listener.
     */
    inline void onAnyMessage(const MessageListener& listener)
    {
        _onAnyMessageListeners.push_back(listener);
    }

    /**
     * @brief Registers listener which receives all messages with commands (messages with leading '/' char).
     * @param commandName Command name which listener can handle.
     * @param listener Listener.
     */
    inline void onCommand(const std::string& commandName, const MessageListener& listener)
    {
        _onCommandListeners[commandName] = listener;
    }

    /**
     * @brief Registers listener which receives all messages with commands (messages with leading '/' char).
     * @param commandsList Commands names which listener can handle.
     * @param listener Listener.
     */
    inline void onCommand(const std::initializer_list<std::string>& commandsList, const MessageListener& listener)
    {
        for (const auto& command : commandsList)
        {
            _onCommandListeners[command] = listener;
        }
    }

    /**
     * @brief Registers listener which receives all messages with commands (messages with leading '/' char) which
     * haven't been handled by other listeners.
     * @param listener Listener.
     */
    inline void onUnknownCommand(const MessageListener& listener)
    {
        _onUnknownCommandListeners.push_back(listener);
    }

    /**
     * @brief Registers listener which receives all messages without commands (messages with no leading '/' char)
     * @param listener Listener.
     */
    inline void onNonCommandMessage(const MessageListener& listener)
    {
        _onNonCommandMessageListeners.push_back(listener);
    }

    /**
     * @brief Registers listener which receives all the inline query.
     * @param listener Listener.
     */
    inline void onInlineQuery(const InlineQueryListener& listener)
    {
        _onInlineQueryListeners.push_back(listener);
    }

    /**
     * @brief Registers listener which receives all the chosen inline result.
     * @param listener Listener.
     */
    inline void onChosenInlineResult(const ChosenInlineResultListener& listener)
    {
        _onChosenInlineResultListeners.push_back(listener);
    }

    inline void onCallbackQuery(const CallbackQueryListener& listener)
    {
        _onCallbackQueryListeners.push_back(listener);
    }

private:
    template <typename ListenerType, typename ObjectType>
    inline void broadcast(const std::vector<ListenerType> &listeners, const ObjectType &object) const
    {
        for (const auto &item : listeners)
            item(object);
    }

    inline void broadcastAnyMessage(const Message& message) const
    {
        broadcast<MessageListener, Message>(_onAnyMessageListeners, message);
    }

    inline bool broadcastCommand(const std::string& command, const Message& message) const
    {
        auto iter = _onCommandListeners.find(command);
        if (iter == _onCommandListeners.end())
            return false;
        iter->second(message);
        return true;
    }

    inline void broadcastUnknownCommand(const Message& message) const
    {
        broadcast<MessageListener, Message>(_onUnknownCommandListeners, message);
    }

    inline void broadcastNonCommandMessage(const Message& message) const
    {
        broadcast<MessageListener, Message>(_onNonCommandMessageListeners, message);
    }

    inline void broadcastInlineQuery(const InlineQuery& query) const
    {
        broadcast<InlineQueryListener, InlineQuery>(_onInlineQueryListeners, query);
    }

    inline void broadcastChosenInlineResult(const ChosenInlineResult& result) const
    {
        broadcast<ChosenInlineResultListener, ChosenInlineResult>(_onChosenInlineResultListeners, result);
    }

    inline void broadcastCallbackQuery(const CallbackQuery& result) const
    {
        broadcast<CallbackQueryListener, CallbackQuery>(_onCallbackQueryListeners, result);
    }

    std::vector<MessageListener> _onAnyMessageListeners;
    std::unordered_map<std::string, MessageListener> _onCommandListeners;
    std::vector<MessageListener> _onUnknownCommandListeners;
    std::vector<MessageListener> _onNonCommandMessageListeners;
    std::vector<InlineQueryListener> _onInlineQueryListeners;
    std::vector<ChosenInlineResultListener> _onChosenInlineResultListeners;
    std::vector<CallbackQueryListener> _onCallbackQueryListeners;

    friend EventHandler;
};

} // namespace TgBot
