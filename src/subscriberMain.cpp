#include <iostream>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include "idl/testmsgPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

class MyListener : public DataReaderListener {
public:
    void on_data_available(DataReader* reader) override {
        SampleInfo info;
        TestMsg msg;
        if (reader->take_next_sample(&msg, &info) == RETCODE_OK) {
            if (info.valid_data) {
                std::cout << "Received: " << msg.message() << " [" << msg.index() << "]" << std::endl;
            }
        }
    }
};

int main() {
    std::cout << "Starting Fast DDS Subscriber..." << std::endl;

    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (participant == nullptr) return 1;

    TypeSupport type(new TestMsgPubSubType());
    type.register_type(participant);

    Topic* topic = participant->create_topic("TestTopic", type.get_type_name(), TOPIC_QOS_DEFAULT);
    Subscriber* subscriber = participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT);

    MyListener listener;
    DataReader* reader = subscriber->create_datareader(topic, DATAREADER_QOS_DEFAULT, &listener);

    std::cout << "Waiting for data, press Enter to stop..." << std::endl;
    std::cin.ignore();

    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return 0;
}
