<?php
/**
 * Auto generated from sequence.proto at 2015-12-02 10:07:29
 *
 * ad.sequence package
 */

namespace Ad\Sequence {
/**
 * SeqResp message
 */
class SeqResp extends \ProtobufMessage
{
    /* Field index constants */
    const SEQ = 1;

    /* @var array Field descriptors */
    protected static $fields = array(
        self::SEQ => array(
            'name' => 'seq',
            'required' => true,
            'type' => 5,
        ),
    );

    /**
     * Constructs new message container and clears its internal state
     *
     * @return null
     */
    public function __construct()
    {
        $this->reset();
    }

    /**
     * Clears message values and sets default ones
     *
     * @return null
     */
    public function reset()
    {
        $this->values[self::SEQ] = null;
    }

    /**
     * Returns field descriptors
     *
     * @return array
     */
    public function fields()
    {
        return self::$fields;
    }

    /**
     * Sets value of 'seq' property
     *
     * @param int $value Property value
     *
     * @return null
     */
    public function setSeq($value)
    {
        return $this->set(self::SEQ, $value);
    }

    /**
     * Returns value of 'seq' property
     *
     * @return int
     */
    public function getSeq()
    {
        return $this->get(self::SEQ);
    }
}
}