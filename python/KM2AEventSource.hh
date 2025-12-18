/**
 * @file KM2AEventSource.hh
 * @brief KM2A模拟数据读取类，用于读取KM2AMCTest.root文件
 * 
 * 提供迭代器和随机索引访问功能，便于通过Nanobind暴露给Python
 */

#ifndef __KM2AEVENTSOURCE_HH__
#define __KM2AEVENTSOURCE_HH__

#include <string>
#include <memory>
#include <stdexcept>

#include "TFile.h"
#include "TTree.h"
#include "LHEvent.h"
#include "KM2AMCEvent.hh"
// ============================================================================
// 合并的事件结构（包含event_info和LHEvent的信息）
// ============================================================================


// ============================================================================
// KM2AEventSource 类
// ============================================================================

/**
 * @brief KM2A事件源类，用于读取ROOT文件中的模拟数据
 * 
 * 支持:
 * - 构造函数直接接收文件名
 * - 迭代器支持for range loop
 * - operator[]支持随机索引访问
 */
class KM2AEventSource {
public:
    // ========================================================================
    // 迭代器类
    // ========================================================================
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = KM2AMCEvent;
        using difference_type = std::ptrdiff_t;
        using pointer = KM2AMCEvent*;
        using reference = KM2AMCEvent&;

        Iterator(KM2AEventSource* source, Long64_t index)
            : source_(source), index_(index) {}

        KM2AMCEvent operator*() const {
            return source_->GetEvent(index_);
        }

        Iterator& operator++() {
            ++index_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++index_;
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return index_ == other.index_;
        }

        bool operator!=(const Iterator& other) const {
            return index_ != other.index_;
        }

    private:
        KM2AEventSource* source_;
        Long64_t index_;
    };

    // ========================================================================
    // 构造函数和析构函数
    // ========================================================================
    
    /**
     * @brief 构造函数，打开ROOT文件并初始化树
     * @param filename ROOT文件路径
     */
    explicit KM2AEventSource(const std::string& filename)
        : filename_(filename) {
        // 打开ROOT文件
        file_ = std::unique_ptr<TFile>(TFile::Open(filename.c_str(), "READ"));
        if (!file_ || file_->IsZombie()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        // 获取event_info树
        event_info_tree_ = dynamic_cast<TTree*>(file_->Get("event_info"));
        if (!event_info_tree_) {
            throw std::runtime_error("Cannot find tree 'event_info' in file: " + filename);
        }

        // 获取event树
        event_tree_ = dynamic_cast<TTree*>(file_->Get("event"));
        if (!event_tree_) {
            throw std::runtime_error("Cannot find tree 'event' in file: " + filename);
        }

        // 设置event_info树的branch地址
        event_info_tree_->SetBranchAddress("eve_id", &br_eve_id_);
        event_info_tree_->SetBranchAddress("eve_trig", &br_eve_trig_);
        event_info_tree_->SetBranchAddress("eve_ene", &br_eve_ene_);
        event_info_tree_->SetBranchAddress("eve_corex", &br_eve_corex_);
        event_info_tree_->SetBranchAddress("eve_corey", &br_eve_corey_);

        // 设置event树的branch地址
        // 注意：对于对象指针，需要传递指针的地址（&lhevent_），ROOT会自动分配内存
        event_tree_->SetBranchAddress("Event", &lhevent_);

        // 获取entries数量
        n_entries_ = event_tree_->GetEntries();
        n_event_info_entries_ = event_info_tree_->GetEntries();
    }

    ~KM2AEventSource() {
        if (file_) {
            file_->Close();
        }
        // lhevent_由ROOT管理，不需要手动删除
    }

    // 禁止拷贝
    KM2AEventSource(const KM2AEventSource&) = delete;
    KM2AEventSource& operator=(const KM2AEventSource&) = delete;

    // 允许移动
    KM2AEventSource(KM2AEventSource&&) = default;
    KM2AEventSource& operator=(KM2AEventSource&&) = default;

    // ========================================================================
    // 访问方法
    // ========================================================================

    /**
     * @brief 获取事件数量
     * @return 事件数量
     */
    Long64_t size() const { return n_entries_; }

    /**
     * @brief 获取event_info树的条目数量
     * @return event_info条目数量
     */
    Long64_t sizeEventInfo() const { return n_event_info_entries_; }

    /**
     * @brief 随机索引访问
     * @param index 事件索引
     * @return KM2AMCEvent结构
     */
    KM2AMCEvent operator[](Long64_t index) {
        return GetEvent(index);
    }

    /**
     * @brief 获取指定索引的事件
     * @param index 事件索引
     * @return KM2AMCEvent结构
     */
    KM2AMCEvent GetEvent(Long64_t index) {
        if (index < 0 || index >= n_entries_) {
            throw std::out_of_range("Event index out of range: " + std::to_string(index));
        }

        KM2AMCEvent data;

        // 读取event_info（如果索引在范围内）
        if (index < n_event_info_entries_) {
            event_info_tree_->GetEntry(index);
            data.eve_id = br_eve_id_;
            data.eve_trig = br_eve_trig_;
            data.eve_ene = br_eve_ene_;
            data.eve_corex = br_eve_corex_;
            data.eve_corey = br_eve_corey_;
        }

        // 读取event树
        event_tree_->GetEntry(index);
        data.lhevent = lhevent_;

        return data;
    }

    // ========================================================================
    // 迭代器支持
    // ========================================================================

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, n_entries_); }

    // ========================================================================
    // 文件信息
    // ========================================================================

    const std::string& filename() const { return filename_; }

private:
    std::string filename_;
    std::unique_ptr<TFile> file_;
    TTree* event_info_tree_ = nullptr;
    TTree* event_tree_ = nullptr;
    Long64_t n_entries_ = 0;
    Long64_t n_event_info_entries_ = 0;

    // LHEvent对象（用于读取event树）
    // 注意：必须使用裸指针并初始化为nullptr，SetBranchAddress需要指针的地址
    LHEvent* lhevent_ = nullptr;

    // event_info树的branch变量
    Int_t br_eve_id_ = 0;
    Int_t br_eve_trig_ = 0;
    Double_t br_eve_ene_ = 0.0;
    Double_t br_eve_corex_ = 0.0;
    Double_t br_eve_corey_ = 0.0;
};

#endif // __KM2AEVENTSOURCE_HH__
