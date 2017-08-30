/*!
    \file matching_engine.cpp
    \brief Matching engine example
    \author Ivan Shynkarenka
    \date 16.08.2017
    \copyright MIT License
*/

#include "trader/matching/market_manager.h"

#include "system/stream.h"

#include <iostream>
#include <regex>
#include <string>

using namespace CppTrader::Matching;

class MyMarketHandler : public MarketHandler
{
protected:
    void onAddSymbol(const Symbol& symbol) override
    { std::cout << "Add symbol: " << symbol << std::endl; }
    void onDeleteSymbol(const Symbol& symbol) override
    { std::cout << "Delete symbol: " << symbol << std::endl; }

    void onAddOrderBook(const OrderBook& order_book) override
    { std::cout << "Add order book: " << order_book << std::endl; }
    void onUpdateOrderBook(const OrderBook& order_book, bool top) override
    { std::cout << "Update order book: " << order_book << (top ? " - Top of the book!" : "") << std::endl; }
    void onDeleteOrderBook(const OrderBook& order_book) override
    { std::cout << "Delete order book: " << order_book << std::endl; }

    void onAddLevel(const OrderBook& order_book, const Level& level, bool top) override
    { std::cout << "Add level: " << level << (top ? " - Top of the book!" : "") << std::endl; }
    void onUpdateLevel(const OrderBook& order_book, const Level& level, bool top) override
    { std::cout << "Update level: " << level << (top ? " - Top of the book!" : "") << std::endl; }
    void onDeleteLevel(const OrderBook& order_book, const Level& level, bool top) override
    { std::cout << "Delete level: " << level << (top ? " - Top of the book!" : "") << std::endl; }

    void onAddOrder(const Order& order) override
    { std::cout << "Add order: " << order << std::endl; }
    void onUpdateOrder(const Order& order) override
    { std::cout << "Update order: " << order << std::endl; }
    void onDeleteOrder(const Order& order) override
    { std::cout << "Delete order: " << order << std::endl; }

    void onExecuteOrder(const Order& order, uint64_t price, uint64_t quantity) override
    { std::cout << "Execute order: " << order << " with price " << price << " and quantity " << quantity << std::endl; }
};

void AddSymbol(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^add symbol (\\d+) (.+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint32_t id = std::stoi(match[1]);
        std::string name = match[2];

        Symbol symbol(id, name.c_str());

        ErrorCode result = market.AddSymbol(symbol);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'add symbol' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'add symbol' command: " << command << std::endl;
}

void DeleteSymbol(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^delete symbol (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint32_t id = std::stoi(match[1]);

        ErrorCode result = market.DeleteSymbol(id);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'delete symbol' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'delete symbol' command: " << command << std::endl;
}

void AddOrderBook(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^add book (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint32_t id = std::stoi(match[1]);

        Symbol symbol(id, "");

        ErrorCode result = market.AddOrderBook(symbol);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'add book' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'add book' command: " << command << std::endl;
}

void DeleteOrderBook(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^delete book (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint32_t id = std::stoi(match[1]);

        ErrorCode result = market.DeleteOrderBook(id);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'delete book' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'delete book' command: " << command << std::endl;
}

void AddMarketOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^add market (buy|sell) (\\d+) (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[2]);
        uint32_t symbol_id = std::stoi(match[3]);
        uint64_t quantity = std::stoi(match[4]);

        Order order;
        if (match[1] == "buy")
            order = Order::BuyMarket(id, symbol_id, quantity);
        else if (match[1] == "sell")
            order = Order::SellMarket(id, symbol_id, quantity);
        else
        {
            std::cerr << "Invalid market order side: " << match[1] << std::endl;
            return;
        }

        ErrorCode result = market.AddOrder(order);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'add market' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'add market' command: " << command << std::endl;
}

void AddSlippageMarketOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^add slippage market (buy|sell) (\\d+) (\\d+) (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[2]);
        uint32_t symbol_id = std::stoi(match[3]);
        uint64_t quantity = std::stoi(match[4]);
        uint64_t slippage = std::stoi(match[5]);

        Order order;
        if (match[1] == "buy")
            order = Order::BuyMarket(id, symbol_id, quantity, slippage);
        else if (match[1] == "sell")
            order = Order::SellMarket(id, symbol_id, quantity, slippage);
        else
        {
            std::cerr << "Invalid market order side: " << match[1] << std::endl;
            return;
        }

        ErrorCode result = market.AddOrder(order);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'add slippage market' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'add slippage market' command: " << command << std::endl;
}

void AddLimitOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^add limit (buy|sell) (\\d+) (\\d+) (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[2]);
        uint32_t symbol_id = std::stoi(match[3]);
        uint64_t price = std::stoi(match[4]);
        uint64_t quantity = std::stoi(match[5]);

        Order order;
        if (match[1] == "buy")
            order = Order::BuyLimit(id, symbol_id, price, quantity);
        else if (match[1] == "sell")
            order = Order::SellLimit(id, symbol_id, price, quantity);
        else
        {
            std::cerr << "Invalid limit order side: " << match[1] << std::endl;
            return;
        }

        ErrorCode result = market.AddOrder(order);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'add limit' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'add limit' command: " << command << std::endl;
}

void AddAONLimitOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^add aon limit (buy|sell) (\\d+) (\\d+) (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[2]);
        uint32_t symbol_id = std::stoi(match[3]);
        uint64_t price = std::stoi(match[4]);
        uint64_t quantity = std::stoi(match[5]);

        Order order;
        if (match[1] == "buy")
            order = Order::BuyLimit(id, symbol_id, price, quantity, true);
        else if (match[1] == "sell")
            order = Order::SellLimit(id, symbol_id, price, quantity, true);
        else
        {
            std::cerr << "Invalid limit order side: " << match[1] << std::endl;
            return;
        }

        ErrorCode result = market.AddOrder(order);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'add aon limit' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'add aon limit' command: " << command << std::endl;
}

void ReduceLimitOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^reduce limit (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[1]);
        uint64_t quantity = std::stoi(match[2]);

        ErrorCode result = market.ReduceOrder(id, quantity);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'reduce limit' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'reduce limit' command: " << command << std::endl;
}

void ModifyLimitOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^modify limit (\\d+) (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[1]);
        uint64_t new_price = std::stoi(match[2]);
        uint64_t new_quantity = std::stoi(match[3]);

        ErrorCode result = market.ModifyOrder(id, new_price, new_quantity);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'modify limit' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'modify limit' command: " << command << std::endl;
}

void ReplaceLimitOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^replace limit (\\d+) (\\d+) (\\d+) (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[1]);
        uint64_t new_id = std::stoi(match[2]);
        uint64_t new_price = std::stoi(match[3]);
        uint64_t new_quantity = std::stoi(match[4]);

        ErrorCode result = market.ReplaceOrder(id, new_id, new_price, new_quantity);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'replace limit' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'replace limit' command: " << command << std::endl;
}

void DeleteOrder(MarketManager& market, const std::string& command)
{
    static std::regex pattern("^delete order (\\d+)$");
    std::smatch match;

    if (std::regex_search(command, match, pattern))
    {
        uint64_t id = std::stoi(match[1]);

        ErrorCode result = market.DeleteOrder(id);
        if (result != ErrorCode::OK)
            std::cerr << "Failed 'delete order' command: " << result << std::endl;

        return;
    }

    std::cerr << "Invalid 'delete order' command: " << command << std::endl;
}

int main(int argc, char** argv)
{
    MyMarketHandler market_handler;
    MarketManager market(market_handler);

    // Perform text input
    std::string line;
    while (getline(std::cin, line))
    {
        if (line == "help")
        {
            std::cout << "Supported commands: " << std::endl;
            std::cout << "add symbol {Id} {Name} - Add a new symbol with {Id} and {Name}" << std::endl;
            std::cout << "delete symbol {Id} - Delete the symbol with {Id}" << std::endl;
            std::cout << "add book {Id} - Add a new order book for the symbol with {Id}" << std::endl;
            std::cout << "delete book {Id} - Delete the order book with {Id}" << std::endl;
            std::cout << "add market {Side} {Id} {SymbolId} {Quantity} - Add a new market order of {Type} (buy/sell) with {Id}, {SymbolId} and {Quantity}" << std::endl;
            std::cout << "add slippage market {Side} {Id} {SymbolId} {Quantity} {Slippage} - Add a new slippage market order of {Type} (buy/sell) with {Id}, {SymbolId}, {Quantity} and {Slippage}" << std::endl;
            std::cout << "add limit {Side} {Id} {SymbolId} {Price} {Quantity} - Add a new limit order of {Type} (buy/sell) with {Id}, {SymbolId}, {Price} and {Quantity}" << std::endl;
            std::cout << "add aon limit {Side} {Id} {SymbolId} {Price} {Quantity} - Add a new 'All-Or-None' limit order of {Type} (buy/sell) with {Id}, {SymbolId}, {Price} and {Quantity}" << std::endl;
            std::cout << "reduce limit {Id} {Quantity} - Reduce the limit order with {Id} by the given {Quantity}" << std::endl;
            std::cout << "modify limit {Id} {NewPrice} {NewQuantity} - Modify the limit order with {Id} and set {NewPrice} and {NewQuantity}" << std::endl;
            std::cout << "replace limit {Id} {NewId} {NewPrice} {NewQuantity} - Replace the limit order with {Id} and set {NewId}, {NewPrice} and {NewQuantity}" << std::endl;
            std::cout << "delete order {Id} - Delete the order with {Id}" << std::endl;
            std::cout << "exit/quit - Exit the program" << std::endl;
        }
        else if ((line == "exit") || (line == "quit"))
            break;
        else if ((line.find("#") == 0) || (line == ""))
            continue;
        else if (line == "enable matching")
            market.EnableMatching();
        else if (line == "disable matching")
            market.DisableMatching();
        else if (line.find("add symbol") != std::string::npos)
            AddSymbol(market, line);
        else if (line.find("delete symbol") != std::string::npos)
            DeleteSymbol(market, line);
        else if (line.find("add book") != std::string::npos)
            AddOrderBook(market, line);
        else if (line.find("delete book") != std::string::npos)
            DeleteOrderBook(market, line);
        else if (line.find("add market") != std::string::npos)
            AddMarketOrder(market, line);
        else if (line.find("add slippage market") != std::string::npos)
            AddSlippageMarketOrder(market, line);
        else if (line.find("add limit") != std::string::npos)
            AddLimitOrder(market, line);
        else if (line.find("add aon limit") != std::string::npos)
            AddAONLimitOrder(market, line);
        else if (line.find("reduce limit") != std::string::npos)
            ReduceLimitOrder(market, line);
        else if (line.find("modify limit") != std::string::npos)
            ModifyLimitOrder(market, line);
        else if (line.find("replace limit") != std::string::npos)
            ReplaceLimitOrder(market, line);
        else if (line.find("delete order") != std::string::npos)
            DeleteOrder(market, line);
        else
            std::cerr << "Unknown command: "  << line << std::endl;
    }

    return 0;
}
