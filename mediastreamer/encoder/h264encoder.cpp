#include "h264encoder.h"

#include <QDebug>



H264Encoder::H264Encoder(QObject *parent)
    : QObject(parent)
{
}

H264Encoder::~H264Encoder()
{
    close();
}

bool H264Encoder::open(int width,
                       int height,
                       int fps,
                       int bitrate)
{
    close();

    m_width  = width;
    m_height = height;

    x264_param_t param;

    x264_param_default_preset(
                &param,
                "ultrafast",
                "zerolatency");

    param.i_width  = width;
    param.i_height = height;

    param.i_fps_num = fps;
    param.i_fps_den = 1;

    // gerçek low latency
    param.i_threads = 1;
    param.i_sync_lookahead = 0;
    param.rc.i_lookahead   = 0;

    // B frame tamamen kapalı
    param.i_bframe = 0;

    // CABAC kapalı
    param.b_cabac = 0;

    // Her frame hemen çıksın
    param.b_sliced_threads = 0;

    // IDR
    param.i_sps_id = 0;
    //param.i_pps_id = 0;

    param.i_keyint_max = fps;
    param.i_keyint_min = fps;

    param.b_repeat_headers = 1;
    param.b_annexb = 1;

    param.i_log_level = X264_LOG_WARNING;

    param.rc.i_rc_method = X264_RC_ABR;
    param.rc.i_bitrate   = bitrate;

    if (x264_param_apply_profile(&param, "baseline") < 0)
    {
        qCritical() << "x264 profile apply failed.";
        return false;
    }

    m_encoder = x264_encoder_open(&param);

    if(!m_encoder)
    {
        qCritical() << "Cannot open x264 encoder.";
        return false;
    }

    x264_picture_init(&m_picOut);

    qDebug() << "H264 encoder initialized.";
    x264_param_t current;
    x264_encoder_parameters(m_encoder, &current);

    qDebug()<<"H264 encode"
        << current.i_width
        << current.i_height;

    return true;
}
void H264Encoder::close()
{
    if(m_encoder)
    {
        x264_encoder_close(m_encoder);
        m_encoder = nullptr;
    }

   /* if(m_picIn.img.plane[0])
    {
        x264_picture_clean(&m_picIn);
    }*/

    m_width = 0;
    m_height = 0;

    m_pts = 0;
}

void H264Encoder::encode(const MediaFrame &frame)
{
    if (!m_encoder)
        return;
    ///qDebug()<< "ENCODE VIDEO"<< frame.pts;

    /*qDebug()
        << "ENCODER OPEN SIZE"
        << m_picIn.img.i_csp
        << m_encoder;
*/
    m_width = frame.width;
    m_height = frame.height;
    /*qDebug()
        << "Encoder:"
        << static_cast<int>(frame.format)
        << frame.width
        << frame.height
        << frame.buffer->stride[0]
        << frame.buffer->stride[1]
        << frame.buffer->stride[2]
        <<frame.pts;*/

    if (frame.format != PixelFormat::YUV420P)
        return;

    if (!frame.buffer || frame.buffer->planeCount != 3)
        return;

    x264_picture_init(&m_picIn);

    m_picIn.img.i_csp   = X264_CSP_I420;
    m_picIn.img.i_plane = 3;

    m_picIn.img.plane[0] = frame.buffer->plane[0];
    m_picIn.img.plane[1] = frame.buffer->plane[1];
    m_picIn.img.plane[2] = frame.buffer->plane[2];

    m_picIn.img.i_stride[0] = frame.buffer->stride[0];
    m_picIn.img.i_stride[1] = frame.buffer->stride[1];
    m_picIn.img.i_stride[2] = frame.buffer->stride[2];

    m_picIn.i_pts = m_pts++;

    x264_nal_t *nals = nullptr;
    int nalCount = 0;

    int frameSize = x264_encoder_encode(m_encoder,
                                        &nals,
                                        &nalCount,
                                        &m_picIn,
                                        &m_picOut);

    if (frameSize <= 0)
        return;

    for (int i = 0; i < nalCount; ++i)
    {
        EncodedPacket packet;

        packet.codec      = CodecType::H264;
        packet.pts        = frame.pts;
        packet.dts        = frame.pts;
        packet.duration   = 0;

        if(nals[i].i_type == NAL_SPS)
        {
            emit spsReady(
                QByteArray(
                    reinterpret_cast<const char*>(nals[i].p_payload),
                    nals[i].i_payload));
        }

        if(nals[i].i_type == NAL_PPS)
        {
            emit ppsReady(
                QByteArray(
                    reinterpret_cast<const char*>(nals[i].p_payload),
                    nals[i].i_payload));
        }


        packet.keyFrame   = (nals[i].i_type == NAL_SLICE_IDR);
        packet.config     = (nals[i].i_type == NAL_SPS ||
                             nals[i].i_type == NAL_PPS);

        packet.endOfFrame = (i == nalCount - 1);

        packet.data = QByteArray(
                    reinterpret_cast<const char*>(nals[i].p_payload),
                    nals[i].i_payload);


       /* qDebug()
            << "RTP PACKET"
            << packet.data.size()
            << packet.keyFrame
            << packet.config;*/


        emit packetReady(packet);
    }
}
