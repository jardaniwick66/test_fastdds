#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include "idl/testmsgPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

class TestMsgSubscriber {
private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    Topic* topic_;
    DataReader* reader_;
    TypeSupport type_;

    class SubListener : public DataReaderListener {
    public:
        SubListener() : samples_(0) {}
        ~SubListener() override {}

        void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override {
            if (info.current_count_change == 1) {
                std::cout << "Subscriber matched." << std::endl;
            } else if (info.current_count_change == -1) {
                std::cout << "Subscriber unmatched." << std::endl;
            } else {
                std::cout << info.current_count_change << " is not a valid value for SubscriptionMatchedStatus current count change." << std::endl;
            }
        }

        void on_data_available(DataReader* reader) override {
            SampleInfo info;
            TestMsg msg;
            if (reader->take_next_sample(&msg, &info) == RETCODE_OK) {
                if (info.valid_data) {
                    samples_++;
                    std::cout << "Message: " << msg.message() << " with index: " << msg.index() << " RECEIVED" << std::endl;
                }
            }
        }

        std::atomic_int samples_;
    } listener_;

public:
    TestMsgSubscriber()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new TestMsgPubSubType()) {
    }

    virtual ~TestMsgSubscriber() {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        }
        if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    bool init() {
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr) return false;

        // Register the Type
        type_.register_type(participant_);

        // Create the Topic
        topic_ = participant_->create_topic("TestMsgTopic", "TestMsg", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) return false;

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        if (subscriber_ == nullptr) return false;

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);
        if (reader_ == nullptr) return false;

        return true;
    }

    void run() {
        std::cout << "Subscriber running. Press Enter to stop" << std::endl;
        std::cin.ignore();
    }
};

int main(int argc, char** argv) {
    std::cout << "Starting subscriber." << std::endl;

    TestMsgSubscriber* mysub = new TestMsgSubscriber();
    if (mysub->init()) {
        mysub->run();
    }

    delete mysub;
    return 0;
}
