#include <iostream>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/topic/Topic.hpp>

#include "idl/testmsgPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

int main() {
    std::cout << "Starting Fast DDS Publisher..." << std::endl;

    // Create Participant
    DomainParticipant* participant = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (participant == nullptr) return 1;

    // Register Type
    TypeSupport type(new TestMsgPubSubType());
    type.register_type(participant);

    // Create Topic
    Topic* topic = participant->create_topic("TestTopic", type.get_type_name(), TOPIC_QOS_DEFAULT);

    // Create Publisher
    Publisher* publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT);

    // Create DataWriter
    DataWriter* writer = publisher->create_datawriter(topic, DATAWRITER_QOS_DEFAULT);

    TestMsg msg;
    msg.index(1);
    msg.message("Hello Fast DDS");

    std::cout << "Sending message: " << msg.message() << " with index " << msg.index() << std::endl;
    writer->write(&msg);

    DomainParticipantFactory::get_instance()->delete_participant(participant);

    return 0;
}