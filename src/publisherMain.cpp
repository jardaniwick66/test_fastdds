#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include "idl/testmsgPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

class TestMsgPublisher {
private:
    TestMsg msg_;
    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* topic_;
    DataWriter* writer_;
    TypeSupport type_;

    class PubListener : public DataWriterListener {
    public:
        PubListener() : matched_(0) {}
        ~PubListener() override {}

        void on_publication_matched(DataWriter*, const PublicationMatchedStatus& info) override {
            if (info.current_count_change == 1) {
                matched_ = info.total_count;
                std::cout << "Publisher matched." << std::endl;
            } else if (info.current_count_change == -1) {
                matched_ = info.total_count;
                std::cout << "Publisher unmatched." << std::endl;
            } else {
                std::cout << info.current_count_change << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
            }
        }

        std::atomic_int matched_;
    } listener_;

public:
    TestMsgPublisher()
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new TestMsgPubSubType()) {
    }

    virtual ~TestMsgPublisher() {
        if (writer_ != nullptr) {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr) {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    bool init() {
        msg_.index(0);
        msg_.message("TestMsg");

        DomainParticipantQos participantQos;
        participantQos.name("Participant_publisher");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr) return false;

        // Register the Type
        type_.register_type(participant_);

        // Create the Topic
        topic_ = participant_->create_topic("TestMsgTopic", "TestMsg", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) return false;

        // Create the Publisher
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr) return false;

        // Create the DataWriter
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
        if (writer_ == nullptr) return false;

        return true;
    }

    bool publish() {
        if (listener_.matched_ > 0) {
            msg_.index(msg_.index() + 1);
            writer_->write(&msg_);
            return true;
        }
        return false;
    }

    void run(uint32_t samples) {
        uint32_t samples_sent = 0;
        while (samples_sent < samples) {
            if (publish()) {
                samples_sent++;
                std::cout << "Message: " << msg_.message() << " with index: " << msg_.index() << " SENT" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

int main(int argc, char** argv) {
    std::cout << "Starting publisher." << std::endl;
    uint32_t samples = 10;

    TestMsgPublisher* mypub = new TestMsgPublisher();
    if (mypub->init()) {
        mypub->run(samples);
    }

    delete mypub;
    return 0;
}